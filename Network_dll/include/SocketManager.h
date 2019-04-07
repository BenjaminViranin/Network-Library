#pragma once

#include <WinSock2.h>
#include <iostream>
#include "Network.h"

class SocketManager
{
private:
	SOCKET			m_sock;
	SOCKADDR_IN		m_sin;
	int				m_sinSize;
	int				PORT;

public:
	SocketManager() ;
	~SocketManager();

	 int				InitServerSocket();
	 int				InitClientSocket(const char* p_serverIP);
	 void				Close();

	 SOCKET&			GetSocket();
	 SOCKADDR_IN&		GetSockador_in();
	 int&				GetSinSize();
};