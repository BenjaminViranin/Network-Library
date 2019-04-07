
#include "include/Network.h"
#include "include/Client.h"
#include "include/Server.h"

std::string oldClientPackage;
void WriteRcvPackageC(Client* c)
{
	while (true)
	{
		std::string test = c->GetLastPackage();
		//if (test != oldClientPackage)
			std::cout << c->GetLastPackage() << "\n";
		oldClientPackage = c->GetLastPackage();
	}
}

std::string oldServerPackage;
void WriteRcvPackageS(Server* s)
{
	while (true)
	{
		std::string test = s->GetLastPackage();
		//if (test != oldServerPackage)
			std::cout << s->GetLastPackage() << "\n";
		oldServerPackage = s->GetLastPackage();
	}
}

int main(int argc, char *argv[])
{
	Network _network = Network();

	int choice;
	std::cout << "[1] Server  [2] Client :  ";
	std::cin >> choice;

	if (choice == 1)
	{
		Server _server;
		_server.Init();
		std::cout << _server.Log("Init") << "\n";

		_server.AddClient();

		_server.StartReceive();

		std::thread t{ WriteRcvPackageS, &_server };

		int	msgSendCount = 0;
		while (true)
		{
			std::cout << _server.Log("AddClient") << "\n";

			//std::cin.get();
			std::string msg = "msg from Server " + std::to_string(msgSendCount);
			++msgSendCount;
			_server.SendToAllClients(msg.c_str());
			std::cout << _server.Log("SendToAllClients") << "\n";
		}
	}
	else if (choice == 2)
	{
		Client _client;
		_client.Init("127.0.0.1");
		std::cout << _client.Log("Init") << "\n";
		std::string clientName = std::to_string(_client.GetSocketManager().GetSockador_in().sin_port);

		while(!_client.ConnectToServer()) {}
		std::cout << _client.Log("ConnectToServer") << "\n";

		_client.StartReceiveFromServer();

		std::thread t{ WriteRcvPackageC, &_client };

		int msgSendCount = 0;
		while (true)
		{
			++msgSendCount;
			//std::cin.get();
			std::string msg = "msg from " + clientName + " " + std::to_string(msgSendCount);
			_client.SendToServer(msg.c_str());
			std::cout << _client.Log("SendToServer") << "\n";
		}
	}

	system("pause");
	return EXIT_SUCCESS;
}
