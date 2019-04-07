#pragma once

#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <map>
#include "SocketManager.h"
#include "Network.h"

class Client
{
private:
	SocketManager					    m_sock;

	std::map<const char*, const char*>  m_logPack;

	const char*						    m_lastPackage;
	std::thread						    m_receiveThread;

public:
	Client();
	Client(const Client& c);
	~Client();

	 int			Init(const char* p_serverIP);
	 void			Close();

	 const char*	Log(const char* p_functionName);

	 bool			ConnectToServer();
	 void			SendToServer(const char* p_package);
	 void			StartReceiveFromServer();

	 const char*	GetLastPackage();
	 SocketManager&	GetSocketManager();

private:
	void			AddLog(const char* p_functionName, const char* p_msg);
	void			ReceiveFromServerThread();
};

extern "C" 
{
	DLL_EXPORT Client*		CreateClient();
	DLL_EXPORT int			InitClient(Client* c, const char* p_serverIP);
	DLL_EXPORT const char*	ClientLog(Client* c, const char* p_functionName);
	DLL_EXPORT bool			ConnectToServer(Client* c);
	DLL_EXPORT void			SendToServer(Client* c, const char* p_package);
	DLL_EXPORT void			StartReceiveFromServer(Client* c);
	DLL_EXPORT const char*	GetLastPackageClient(Client* c);
}