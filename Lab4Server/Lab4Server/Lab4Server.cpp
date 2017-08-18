// Lab4Server.cpp : Defines the entry point for the console application.
//

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "structs.h"
#include <vector>
#include "ClientUser.h"
#include "ServerLobby.h"
#include "ServerOut.h"
#include "World.h"
#include "Actor.h"
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

SOCKET ListenSocket = INVALID_SOCKET;
addrinfo *result = NULL, *ptr = NULL, hints;

std::vector<ClientUser*> Users;
std::vector<Actor> Actors;
ServerLobby* Lobby;
ServerOut* Tick;
World* GameWorld;
WSADATA wsaData;
void init();

int main()
{
	init();
	
	Lobby = new ServerLobby(ListenSocket, &Users, &Actors);
	Tick = new ServerOut(ListenSocket, &Users, &Actors);
	GameWorld = new World(&Users, &Actors);

	GameWorld->Start();
	Lobby->Start();
	Tick->Start();

	while(true)
	{
		using namespace std::chrono_literals;
		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
	}

    return 0;
}

void init()
{
	printf("Starting server... \n");

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	int yes = 1;
	iResult = setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int));
	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	printf("Socket creation success \n");
}

