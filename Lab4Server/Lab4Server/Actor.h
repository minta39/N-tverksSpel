#pragma once
#include <vector>

class Actor
{


private:
	std::vector<int> Location;
	std::vector<int> Size;
	int ID = 0;
public:
	std::vector<int>* GetLocation();
	void SetLocation(int X, int Y);
	void SetID(int id);
	int GetID();
	Actor();
	~Actor();
};

