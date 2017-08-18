#ifndef ACTOR_H
#define ACTOR_H
#include <vector>
#include "structs.h"

class Actor
{

private:
    std::vector<int> Location;
    std::vector<int> Size;
    int ID = 0;
    char Name[MAXNAMELEN];
public:
    Actor();
    std::vector<int>* GetLocation();
    void SetLocation(int X, int Y);
    void SetID(int id);
    int GetID();
    void SetName(char name[MAXNAMELEN]);
    char* GetName();
};

#endif // ACTOR_H
