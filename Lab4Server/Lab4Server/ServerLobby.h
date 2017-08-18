#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "structs.h"
#include <vector>
#include "ClientUser.h"
#include "GameThread.h"
#include "Actor.h"

#pragma comment(lib, "Ws2_32.lib")

class ServerLobby : public GameThread
{
public:
	ServerLobby(SOCKET ListenSocket, std::vector<ClientUser*>* Users, std::vector<Actor>* Actors);
	~ServerLobby();

private:
	virtual void run() override;
	SOCKET ListenSocket;
	std::vector<ClientUser*>* Users;
	std::vector<Actor>* Actors;
	
};

