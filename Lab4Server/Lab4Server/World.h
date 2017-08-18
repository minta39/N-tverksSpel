#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "structs.h"
#include <vector>
#include "ClientUser.h"
#include "GameThread.h"

#pragma comment(lib, "Ws2_32.lib")

class World : public GameThread
{
public:
	World(std::vector<ClientUser*>* Users, std::vector<Actor>* Actors);
	~World();

private:
	virtual void run() override;
	std::vector<ClientUser*>* Users;
	std::vector<Actor>* Actors;
	bool Collide(int x, int y, Actor* B);
	Actor* GetClientActor(ClientUser* User);
};

