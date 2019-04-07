#include "../include/SocketManager.h"
#include <WS2tcpip.h>

SocketManager::SocketManager() : m_sinSize(sizeof m_sin), PORT(9842)
{
}

SocketManager::~SocketManager()
{
	Close();
}

int SocketManager::InitServerSocket()
{
	if ((m_sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "\n";
		perror("socket()");
		std::cout << "INVALID_SOCKET" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	std::cout << "Init Socket :: ";

	/* Bind */
	m_sin.sin_family = AF_INET;
	m_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sin.sin_port = htons(PORT);

	if (bind(m_sock, (SOCKADDR *)&m_sin, sizeof m_sin) == SOCKET_ERROR)
	{
		std::cout << "\n";
		perror("bind()");
		std::cout << "SOCKET_ERROR" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	std::cout << "Bind Socket :: \n";

	return EXIT_SUCCESS;
}

int SocketManager::InitClientSocket(const char* p_serverIP)
{
	if ((m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{

		std::cout << "\n";
		perror("socket()");
		std::cout << "INVALID_SOCKET" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	std::cout << "Init Socket :: ";

	/* Bind */
	m_sin = { 0 };
	m_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(PORT);

	//setup address structure
	memset((char *)&m_sin, 0, sizeof(m_sin));

	inet_pton(AF_INET, p_serverIP, &m_sin.sin_addr.s_addr);
	char str[15];
	inet_ntop(AF_INET, &m_sin.sin_addr.s_addr, str, 15);

	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(PORT);

	return EXIT_SUCCESS;
}

void SocketManager::Close()
{
	std::cout << "Close Socket :: ";
	closesocket(m_sock);
}

SOCKET& SocketManager::GetSocket()
{
	return m_sock;
}

SOCKADDR_IN& SocketManager::GetSockador_in()
{
	return m_sin;
}

int& SocketManager::GetSinSize()
{
	m_sinSize = sizeof m_sin;
	return m_sinSize;
}