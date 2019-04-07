#pragma once

#include <WinSock2.h>
#include <iostream>

#define DLL_EXPORT __declspec(dllexport)

class Network
{
private:
	WSADATA m_wsa;

public:
	Network();
	~Network();

	 int	Init();
	 void	Destroy();
};