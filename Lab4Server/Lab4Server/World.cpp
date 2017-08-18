#include "stdafx.h"
#include "World.h"
#include <thread>
#include <vector>

World::World(std::vector<ClientUser*>* Users, std::vector<Actor>* Actors)
{
	this->Users = Users;
	this->Actors = Actors;
}


World::~World()
{
}

void World::run()
{
	long time = 0;
	printf("World tick started\n");
	while(true)
	{
		using namespace std::chrono_literals;
		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(17 - time));
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		time = elapsed.count() - 17;

		for (int i = 0; i < Users->capacity(); i++)
		{
			Actor* actor;
			if (Users->at(i)->IsSetupDone() && Users->at(i)->Connected() && Users->at(i)->Joined() && Users->at(i)->GetID() != 0)
			{
				bool Collided = false;
				actor = GetClientActor(Users->at(i));
				std::vector<int>* location = Users->at(i)->move();
				if (location == NULL) continue;
				printf("x position: %d\n", location->at(0));
				printf("y position: %d\n", location->at(1));
				if (location->at(0) < -100 || location->at(1) > 80 || location->at(0) > 80 || location->at(1) < -100) continue;
				for (int n = 0; n < Actors->capacity(); n++)
				{
					if (Actors->at(n).GetID() != Users->at(i)->GetID())
					if (Collide(location->at(0), location->at(1), &Actors->at(n)))
					{
						Collided = true;
						break;
					}
				}
				if (Collided) continue;
				actor->SetLocation(location->at(0), location->at(1));
			}
		}

	}
}

Actor* World::GetClientActor(ClientUser* User)
{
	for (int i = 0; i < Actors->capacity(); i++ )
	{
		if (User->GetID() == Actors->at(i).GetID())
		{
			return &Actors->at(i);
		}
	}
}


bool World::Collide(int x, int y, Actor* B)
{
	std::vector<int>* location = B->GetLocation();
	if (x < location->at(0) + 20 && x + 20 > location->at(1) && y < location->at(1) + 20 && 20 + y > location->at(1)) return true;
	else return false;
}