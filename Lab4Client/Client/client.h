#ifndef CLIENT_H
#define CLIENT_H

#include "Data/world.h"
#include "structs.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#include "structs.h"

#define DEFAULT_BUFLEN 512

class Client
{
public:
    Client();
    World* GetWorld(); // Returns a pointer of the world
    void SetWorld(World *world);

    void Connect(std::string Ip, int Port);
    void JoinServer();
    void LeaveServer();
    void EventToServer(MoveEvent moveEvent);

    void TcpListen();
    void UdpListen();

    void StartThread();

private:
    World* world = NULL;

    WSADATA WsaData;

    addrinfo *result = NULL, *ptr = NULL, hints;

    SOCKET ConnectSocket = INVALID_SOCKET;

    unsigned int SequenceNumber = 0;
    unsigned int ClientID = 0;
    unsigned int ServerSequenceNumber = 0;

};

#endif // CLIENT_H
