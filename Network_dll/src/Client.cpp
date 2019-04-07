#include "../include/Client.h"

DLL_EXPORT Client* CreateClient()
{
	return new Client();
}

DLL_EXPORT int InitClient(Client* c, const char* p_serverIP)
{
	return c->Init(p_serverIP);
}

DLL_EXPORT const char* ClientLog(Client* c, const char* p_functionName)
{
	return c->Log(p_functionName);
}

DLL_EXPORT bool ConnectToServer(Client* c)
{
	return c->ConnectToServer();
}

DLL_EXPORT void SendToServer(Client* c, const char * p_package)
{
	c->SendToServer(p_package);
}

DLL_EXPORT void StartReceiveFromServer(Client * c)
{
	c->StartReceiveFromServer();
}

DLL_EXPORT const char* GetLastPackageClient(Client * c)
{
	return c->GetLastPackage();
}

Client::Client()
{
	m_lastPackage = new char[4096] {'\0'};
}

Client::Client(const Client& c)
{
	m_lastPackage = new char[4096] { '\0' };
}

Client::~Client()
{
	Close();
}

int Client::Init(const char* p_serverIP)
{
	AddLog("Init", "Init Client :: ");
	return m_sock.InitClientSocket(p_serverIP);
}

void Client::Close()
{
	AddLog("Close", "Close Client :: ");
	m_receiveThread.join();
}

const char* Client::Log(const char* p_functionName)
{
	if (m_logPack.find(p_functionName) != m_logPack.end())
		return m_logPack[p_functionName];
	return "log empty";
}

void Client::AddLog(const char* p_functionName, const char* p_msg)
{
	if (m_logPack.find(p_functionName) != m_logPack.end())
		m_logPack[p_functionName] = p_msg;
	else
		m_logPack.emplace(p_functionName, p_msg);
}

bool Client::ConnectToServer()
{
	char buffer[4096] = "\0";
	ZeroMemory(buffer, 4096);
	
	SendToServer("ITTHACWY");

	int byteIn = recvfrom(m_sock.GetSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *) &m_sock.GetSockador_in(), &m_sock.GetSinSize());
	if (byteIn != SOCKET_ERROR)
	{
		std::string s = buffer;
		if (s == "connected")
		{
			AddLog("ConnectToServer", "connected");
			return true;
		}
	}
	
	return false;
}

void Client::SendToServer(const char* p_package)
{
	std::string msg = p_package;
	if (sendto(m_sock.GetSocket(), msg.c_str(), msg.size()+1, 0,
		(sockaddr*)&m_sock.GetSockador_in(), m_sock.GetSinSize()) == SOCKET_ERROR)
	{
		AddLog("SendToServer", "error: can't send");
	}
}

void Client::StartReceiveFromServer()
{
	m_receiveThread = std::thread{ &Client::ReceiveFromServerThread, this };
}

void Client::ReceiveFromServerThread()
{
	char buffer[4096];
	ZeroMemory(buffer, 4096);
	while (true)
	{
		int byteIn = recvfrom(m_sock.GetSocket(), buffer, sizeof(buffer), 0, 
							 (struct sockaddr *) &m_sock.GetSockador_in(), &m_sock.GetSinSize());

		if (byteIn != SOCKET_ERROR)
			m_lastPackage = buffer;
	}
}

const char* Client::GetLastPackage()
{
	return m_lastPackage;
}

SocketManager& Client::GetSocketManager()
{
	return m_sock;
}
