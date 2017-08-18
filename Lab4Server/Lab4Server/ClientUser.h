#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "Actor.h"
#include "GameThread.h"
#define DEFAULT_BUFLEN 1024

#pragma comment(lib, "Ws2_32.lib")

class ClientUser : public GameThread
{
public:
	ClientUser();
	~ClientUser();

	void SetSocket(SOCKET socket);
	SOCKET* GetSocket();
	bool Connected();
	void Disconnect();
	bool Joined();
	void SetJoined(bool join);
	void SetID(int id);
	int GetID();
	std::vector<int>* move();
	void SetupDone();
	bool IsSetupDone();
	void IncSeqNum();
	unsigned int GetSequenceNumber();
	std::vector<int> location;
private:
	unsigned int SequenceNumber = 0;
	bool bSetupDone = false;
	bool bMove = false;
	
	int ID = 0;
	bool IsConnected = false;
	bool HasJoined = false;
	SOCKET ClientSocket;
	virtual void run() override;
};

