#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "structs.h"
#include <vector>
#include "ClientUser.h"
#include "GameThread.h"

#pragma comment(lib, "Ws2_32.lib")

class ServerOut : public GameThread
{
public:
	ServerOut(SOCKET ListenSocket, std::vector<ClientUser*>* Users, std::vector<Actor>* Actors);
	~ServerOut();

private:
	virtual void run() override;
	SOCKET ListenSocket;
	std::vector<ClientUser*>* Users;
	std::vector<Actor>* Actors;
	void SetID(ClientUser* User);
	void SendNewPlayer(ClientUser* User);
	void SendLeavePlayer(ClientUser* User);
};

