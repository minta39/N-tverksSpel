#include "actor.h"
#include "Data/world.h"

Actor::Actor()
{
    Location.resize(2);
    Location.at(0) = 0;
    Location.at(1) = 0;
    Size.resize(2);
    Size.at(0) = 20;
    Size.at(1) = 20;
}

std::vector<int>* Actor::GetLocation()
{
    return &Location;
}

void Actor::SetLocation(int X, int Y)
{
    Location.at(0) = X;
    Location.at(1) = Y;
}

int Actor::GetID()
{
    return ID;
}
void Actor::SetID(int id)
{
    ID = id;
}

void Actor::SetName(char name[MAXNAMELEN])
{
    strcpy(Name, name);
}

char* Actor::GetName()
{
    return Name;
}
