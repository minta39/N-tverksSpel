#include "stdafx.h"
#include "ServerOut.h"
#include <thread>


ServerOut::ServerOut(SOCKET ListenSocket, std::vector<ClientUser*>* Users, std::vector<Actor>* Actors)
{
	this->ListenSocket = ListenSocket;
	this->Users = Users;
	this->Actors = Actors;
}


ServerOut::~ServerOut()
{
}

void ServerOut::run()
{
	long time = 0;
	printf("Server tick started\n");
	while (true)
	{
		using namespace std::chrono_literals;
		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(17 - time));
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		time = elapsed.count() - 17;

		for (int i = 0; i < Users->capacity(); i++)
		{
			if (!Users->at(i)->Connected() && !Users->at(i)->IsSetupDone())
			{
				SendLeavePlayer(Users->at(i));
				continue;
			}
			if (!Users->at(i)->IsSetupDone() &&Users->at(i)->Joined()) continue;
			if (Users->at(i)->GetID() == 0)
			{
				printf("Setting ID forn new player... \n");
				SetID(Users->at(i));
				SendNewPlayer(Users->at(i));
				continue;
			}
			for (int n = 0; n < Actors->capacity(); n++)
			{
				int iResult;
				NewPlayerPositionMsg nppm;
				nppm.msg.type = NewPlayerPosition;
				nppm.msg.head.id = Actors->at(n).GetID();
				nppm.msg.head.seq_no = Users->at(i)->GetSequenceNumber();
				nppm.msg.head.length = sizeof(nppm);
				nppm.pos.x = Actors->at(n).GetLocation()->at(0);
				nppm.pos.y = Actors->at(n).GetLocation()->at(1);

				char sendbuf[sizeof(nppm)];
				memcpy((void*)sendbuf, (void*)&nppm, sizeof(nppm));
				iResult = send(*Users->at(i)->GetSocket(), sendbuf, sizeof(sendbuf), 0);
				if (iResult == SOCKET_ERROR)
				{
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(*Users->at(i)->GetSocket());
					Users->at(i)->Disconnect();
				}
				Users->at(i)->IncSeqNum();
			}
		}

	}
}

void ServerOut::SendLeavePlayer(ClientUser* User)
{
	PlayerLeaveMsg plm;
	plm.msg.head.id = User->GetID();
	plm.msg.type = PlayerLeave;

	char sendbuff[sizeof(plm)];

	int iResult;
	for (int i = 0; i < Users->capacity(); i++)
	{
		if (Users->at(i)->GetID() != 0)
		{
			plm.msg.head.seq_no = Users->at(i)->GetSequenceNumber();
			plm.msg.head.length = sizeof(plm);
			memcpy((void*)sendbuff, (void*)&plm, sizeof(plm));
			printf("Sending leave players to client ... \n");
			iResult = send(*Users->at(i)->GetSocket(), sendbuff, sizeof(sendbuff), 0);
			if (iResult == SOCKET_ERROR)
			{
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(*Users->at(i)->GetSocket());
				Users->at(i)->Disconnect();
			}
		}
	}
}

void ServerOut::SendNewPlayer(ClientUser* User)
{

	NewPlayerMsg npm;
	npm.msg.head.id = User->GetID();
	npm.msg.type = NewPlayer;

	char sendbuff[sizeof(npm)];

	int iResult;
	for (int i = 0; i < Users->capacity(); i++)
	{
		if (Users->at(i)->GetID() != 0)
		{
			npm.msg.head.seq_no = Users->at(i)->GetSequenceNumber();
			npm.msg.head.length = sizeof(npm);
			memcpy((void*)sendbuff, (void*)&npm, sizeof(npm));
			printf("Sending new players to client1 ... \n");
			iResult = send(*Users->at(i)->GetSocket(), sendbuff, sizeof(sendbuff), 0);
			printf("Sending new players to client1 ... done \n");
		}
	}
	for (int i = 0; i < Users->capacity(); i++)
	{
		if (Users->at(i)->GetID() != User->GetID())
		{
			NewPlayerMsg npm;
			npm.msg.head.id = Users->at(i)->GetID();
			npm.msg.type = NewPlayer;

			char sendbuf[sizeof(npm)];
			npm.msg.head.seq_no = User->GetSequenceNumber();
			npm.msg.head.length = sizeof(npm);
			memcpy((void*)sendbuf, (void*)&npm, sizeof(npm));
			printf("Sending new players to client2 ... \n");
			iResult = send(*User->GetSocket(), sendbuf, sizeof(sendbuf), 0);	
			if (iResult == SOCKET_ERROR)
			{
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(*Users->at(i)->GetSocket());
				Users->at(i)->Disconnect();
			}
			User->IncSeqNum();
			printf("Sending new players to client2 ... done \n");
		}
	}
}

void ServerOut::SetID(ClientUser* User)
{
	bool IDAvailable = true;
	Actors->push_back(Actor());
	for (int i = 0; i < Actors->capacity(); i++) {
		IDAvailable = true;
		for (int n = 0; n < Actors->capacity(); n++) {
			printf("ID's: %d\n", Actors->at(n).GetID());
			if (Actors->at(n).GetID() == i + 1) {
				IDAvailable = false;
			}
		}
		if (IDAvailable) {
			printf("Address in out: %d\n", &Actors->at(0));
			Actors->at(Actors->capacity() - 1).SetID(i +1);
			User->SetID(i + 1);
			int iResult;

			MsgHead msg;
			msg.id = i + 1;
			msg.type = Join;
			msg.length = sizeof(msg);

			char sendbuf[sizeof(msg)];

			memcpy((void*)sendbuf, (void*)&msg, sizeof(msg));

			iResult = send(*User->GetSocket(), sendbuf, sizeof(sendbuf), 0);
			if (iResult == SOCKET_ERROR)
			{
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(*User->GetSocket());
				User->Disconnect();
				return;
			}
			printf("New user ID: %d\n", User->GetID());
			User->IncSeqNum();
			User->SetJoined(true);

			break;
		}
	}
}