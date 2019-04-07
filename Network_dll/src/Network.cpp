#include "../include/Network.h"

Network::Network()
{
	Init();
}

Network::~Network()
{
	Destroy();
}

int Network::Init()
{
	int err = WSAStartup(MAKEWORD(2, 2), &m_wsa);
	if (err < 0)
	{
		std::cout << "WSAStartup failed !" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	std::cout << ":: Init Network :: ";
}

void Network::Destroy()
{
	std::cout << "Destroy Network :: ";
	WSACleanup();
}