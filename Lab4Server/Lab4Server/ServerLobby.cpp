#include "stdafx.h"
#include "ServerLobby.h"
#include <thread>


ServerLobby::ServerLobby(SOCKET ListenSocket, std::vector<ClientUser*>* Users, std::vector<Actor>* Actors)
{
	this->ListenSocket = ListenSocket;
	this->Users = Users;
	this->Actors = Actors;
}


ServerLobby::~ServerLobby()
{
}

void ServerLobby::run()
{
	SOCKET ClientSocket = INVALID_SOCKET;
	std::vector<SOCKET> vSocket;
	printf("Lobby tick started\n");
	while (true)
	{
		printf("Waiting for clients... \n");
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			//closesocket(ClientSocket);
			continue;
		}	
		printf("Accepted Client ... \n");
		//ClientUser* user = new ClientUser();	
		//user->SetSocket(ClientSocket);	
		//Users->push_back(*user);
		printf("Client Socket name in LOBBY?: %d\n", ClientSocket);
		Users->push_back(new ClientUser());
		Users->at(Users->capacity() - 1)->SetSocket(ClientSocket);

		Users->at(Users->capacity() - 1)->Start();
		
		Users->at(Users->capacity() - 1)->SetupDone();
		//Users->push_back(ClientUser());
		//ClientSocket = INVALID_SOCKET;

		printf("New client added ... \n");

	}
}
