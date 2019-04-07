#include "../include/Server.h"

DLL_EXPORT Server* CreateServer()
{
	return new Server();
}

DLL_EXPORT int InitServer(Server* s)
{
	return s->Init();
}
DLL_EXPORT const char* ServerLog(Server* s, const char* p_functionName)
{
	return s->Log(p_functionName);
}
DLL_EXPORT void AddClient(Server* s, int max)
{
	s->AddClient(max);
}
DLL_EXPORT bool IsAddingClient(Server* s)
{
	return s->IsAddingClient();
}
DLL_EXPORT void StopAddClient(Server* s)
{
	s->StopAddClient();
}
DLL_EXPORT void StartReceive(Server* s)
{
	s->StartReceive();
}
DLL_EXPORT void SendTo(Server* s, USHORT p_clientPort, const char* p_package)
{
	s->SendTo(p_clientPort, p_package);
}
DLL_EXPORT void SendToAllClients(Server* s, const char* p_package)
{
	s->SendToAllClients(p_package);
}
DLL_EXPORT int GetClientNumber(Server* s)
{
	return s->GetClientNumber();
}
DLL_EXPORT const char* GetLastPackageServer(Server* s)
{
	return s->GetLastPackage();
}
DLL_EXPORT USHORT * GetClientsPort(Server* s)
{
	return s->GetClientsPort();
}

Server::Server() : m_clientNumber(0), m_isAddingClient(true)
{
	m_lastPackage = new char[4096] { '\0' };
}

Server::~Server()
{
	Close();
}

int Server::Init()
{
	AddLog("Init", "Init Server :: ");
	return m_serverSock.InitServerSocket();
}

void Server::Close()
{
	AddLog("Close", ":: Close Server ::");
	m_receiveThread.join();
	m_addClientThread.join();
}

const char* Server::Log(const char* p_functionName)
{
	if (m_logPack.find(p_functionName) != m_logPack.end())
		return m_logPack[p_functionName];
	return "log empty";
}

void Server::AddLog(const char* p_functionName, const char* p_msg)
{
	if (m_logPack.find(p_functionName) != m_logPack.end())
		m_logPack[p_functionName] = p_msg;
	else
		m_logPack.emplace(p_functionName, p_msg);
}

void Server::AddClient(int max)
{
	m_addClientThread = std::thread{ &Server::AddClientThread, this, max };
}

void Server::AddClientThread(int max)
{
	char buffer[4096];
	ZeroMemory(buffer, 4096);
	while (m_isAddingClient)
	{
		int byteIn = recvfrom(m_serverSock.GetSocket(), buffer, sizeof(buffer), 0,
			(sockaddr *)&m_serverSock.GetSockador_in(), &m_serverSock.GetSinSize());

		if (byteIn != SOCKET_ERROR)
		{
			std::string msg = buffer;
			if (msg == "ITTHACWY")
			{
				bool isNewClient = true;
				for (int i = 0; i < m_clientPack.size(); ++i)
				{
					if (m_clientPack[i].sin_port == m_serverSock.GetSockador_in().sin_port)
					{
						AddLog("AddClient", "Not a new Client");
						isNewClient = false;
						break;
					}
				}
				if (isNewClient)
				{
					m_clientPack.push_back(m_serverSock.GetSockador_in());
					++m_clientNumber;
					SendTo(m_clientPack.back(), "connected");
					AddLog("AddClient", "Client Added");
				}
			}
		}
		if (m_clientNumber == max)
			m_isAddingClient = false;
	}
}

bool Server::IsAddingClient()
{
	return m_isAddingClient;
}

void Server::StopAddClient()
{
	m_isAddingClient = false;
}

int Server::GetClientNumber()
{
	return m_clientNumber;
}

USHORT* Server::GetClientsPort()
{
	USHORT* clientsPort = new USHORT[m_clientPack.size()];
	for (int i = 0; i < m_clientPack.size(); ++i)
	{
		clientsPort[i] = m_clientPack[i].sin_port;
	}
	return clientsPort;
}

char* Server::GetLastPackage()
{
	return _strdup(m_lastPackage);
}

void Server::StartReceive()
{
	m_receiveThread = std::thread{ &Server::ReceiveThread, this };
}

void Server::ReceiveThread()
{
	char buffer[4096];
	ZeroMemory(buffer, 4096);
	while (true)
	{
		int byteIn = recvfrom(m_serverSock.GetSocket(), buffer, sizeof(buffer), 0, 
							 ( sockaddr *) &m_serverSock.GetSockador_in(), &m_serverSock.GetSinSize());

		if (byteIn != SOCKET_ERROR)
			m_lastPackage = buffer;
	}
}

void Server::SendTo(SOCKADDR_IN p_clientSin, const char* p_package)
{
	std::string msg = p_package;
	if (sendto(m_serverSock.GetSocket(), msg.c_str(), msg.size() + 1, 0,
		(sockaddr*)&p_clientSin, sizeof(p_clientSin)) == SOCKET_ERROR)
	{
		AddLog("SendTo", "error: can't send");
	}
}

void Server::SendTo(USHORT p_clientPort, const char* p_package)
{
	std::string msg = p_package;
	for (int i = 0; i < m_clientPack.size(); ++i)
	{
		if (p_clientPort == m_clientPack[i].sin_port)
		{
			if (sendto(m_serverSock.GetSocket(), msg.c_str(), msg.size() + 1, 0,
				(sockaddr*)&m_clientPack[i], sizeof(m_clientPack[i])) == SOCKET_ERROR)
			{
				AddLog("SendTo", "error: can't send");
			}
			break;
		}
	}
}

void Server::SendToAllClients(const char* p_package)
{
	std::string msg = p_package;
	for(int i = 0; i < m_clientPack.size(); ++i)
	{
		if (sendto(m_serverSock.GetSocket(), msg.c_str(), msg.size() + 1, 0,
			(sockaddr*)&m_clientPack[i], sizeof(m_clientPack[i])) == SOCKET_ERROR)
		{
			AddLog("SendToAllClients", "error: can't send");
		}
	}
}
