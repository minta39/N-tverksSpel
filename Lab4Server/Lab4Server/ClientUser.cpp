#include "stdafx.h"
#include "ClientUser.h"
#include <thread>
#include "structs.h"

ClientUser::ClientUser()
{
	location.resize(2);
	location.at(0) = 0;
	location.at(1) = 0;
}


ClientUser::~ClientUser()
{
}

void ClientUser::SetSocket(SOCKET socket)
{
	ClientSocket = socket;
	IsConnected = true;
	printf("Client Socket Address name in set socket?: %d\n", ClientSocket);
}

SOCKET* ClientUser::GetSocket()
{
	return &ClientSocket;
}

bool ClientUser::Connected()
{
	return IsConnected;
}

void ClientUser::Disconnect()
{
	IsConnected = false;
}

void ClientUser::run()
{
	long time = 0;

	while (true)
	{
		int iResult = 0;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		using namespace std::chrono_literals;
		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(17 - time));
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		time = elapsed.count() - 17;

		iResult = recv(*GetSocket(), recvbuf, recvbuflen, 0);	
		if (iResult > 0)
		{

			printf("Client Socket name in run?: %d\n", *GetSocket());
			printf("Bytes received: %d\n", iResult);
			MsgHead* msghead;
			msghead = (MsgHead*)recvbuf;
			if (ID == 0 && msghead->type == Join)
			{
				printf("Recieved MsgHead Join... \n");

				SetJoined(true);
				continue;
			}

			if (msghead->type == Event)
			{
				printf("Recieved MsgHead Event... \n");
				MoveEvent *me;
				me = (MoveEvent*)recvbuf;
				bMove = true;
				location.at(0) = me->pos.x;
				location.at(1) = me->pos.y;

			}else if (msghead->type == Leave)
			{
				printf("Recieved MsgHead Leave... \n");
				LeaveMsg *me;
				me = (LeaveMsg*)recvbuf;
				Disconnect();

			}
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
			Disconnect();
		}
		else
			printf("recv failed: %d\n", WSAGetLastError());

	}
}

void ClientUser::SetJoined(bool join)
{
	HasJoined = join;
}

bool ClientUser::Joined()
{
	return HasJoined;
}

int ClientUser::GetID()
{
	return ID;
}
void ClientUser::SetID(int id)
{
	this->ID = id;
}

std::vector<int>* ClientUser::move()
{
	if (bMove)
	{
		bMove = false;
		return &location;
	}
	return NULL;
}

void ClientUser::SetupDone()
{
	bSetupDone = true;
}

bool ClientUser::IsSetupDone()
{
	return bSetupDone;
}

void ClientUser::IncSeqNum()
{
	SequenceNumber++;
}

unsigned int ClientUser::GetSequenceNumber()
{
	return SequenceNumber;
}