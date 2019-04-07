#pragma once

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include "SocketManager.h"
#include "Network.h"

class Server
{
private:

	SocketManager						m_serverSock;

	std::map<const char*, const char*>	m_logPack;

	int									m_clientNumber;
	std::vector<SOCKADDR_IN>			m_clientPack;

	const char*							m_lastPackage;

	bool								m_isAddingClient;
	std::thread							m_addClientThread;
	std::thread							m_receiveThread;

public:
	Server();
	~Server();

	 int			Init();
	 void			Close();

	 const char*	Log(const char* p_functionName);

	 void			AddClient(int max = UINT8_MAX);
	 bool			IsAddingClient();
	 void			StopAddClient();

	 void			StartReceive();
	 void			SendTo(SOCKADDR_IN p_clientSin, const char* p_package);
	 void			SendTo(USHORT p_clientPort, const char* p_package);
	 void			SendToAllClients(const char* p_package);

	 int			GetClientNumber();
	 char*			GetLastPackage();
	 USHORT*		GetClientsPort();

private:
	void			AddLog(const char* p_functionName, const char* p_msg);
	void			AddClientThread(int max);
	void			ReceiveThread();
};

extern "C"
{
	DLL_EXPORT Server*		CreateServer();
	DLL_EXPORT int			InitServer(Server* s);
	DLL_EXPORT const char*	ServerLog(Server* s, const char* p_functionName);
	DLL_EXPORT void			AddClient(Server* s, int max = UINT8_MAX);
	DLL_EXPORT bool			IsAddingClient(Server* s);
	DLL_EXPORT void			StopAddClient(Server* s);
	DLL_EXPORT void			StartReceive(Server* s);
	DLL_EXPORT void			SendTo(Server* s, USHORT p_clientPort, const char* p_package);
	DLL_EXPORT void			SendToAllClients(Server* s, const char* p_package);
	DLL_EXPORT int			GetClientNumber(Server* s);
	DLL_EXPORT const char*	GetLastPackageServer(Server* s);
	DLL_EXPORT USHORT*		GetClientsPort(Server* s);
}
