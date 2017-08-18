#include "client.h"
#include "structs.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "Data/world.h"


Client::Client()
{
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &WsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
}

World* Client::GetWorld()
{
    return world;
}

void Client::SetWorld(World *world)
{
    this->world = world;
}

void Client::Connect(std::string Ip, int Port)
{
    int iResult = 0;

    iResult = getaddrinfo(Ip.c_str(), std::to_string(Port).c_str(), &hints, &result);
    if (iResult != 0)
    {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return;
    }
    ptr = result;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        printf("Server closed");
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }
    JoinServer();
}

void Client::JoinServer()
{
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to join server: INVALID_SOCKET!\n");
        WSACleanup();
        return;
    }

    int iResult;

    JoinMsg join;
    char sendbuf[sizeof(join)];

    join.head.id = 0;
    join.head.length = sizeof(join);
    join.head.type = Join;

    memcpy((void*)sendbuf, (void*)&join, sizeof(join));

    iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
}

void Client::LeaveServer()
{
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }

    int iResult;
    LeaveMsg leave;
    char sendbuf[sizeof(leave)];
    leave.head.id = ClientID;
    leave.head.length = sizeof(leave);
    leave.head.seq_no = SequenceNumber;
    leave.head.type = Leave;

    memcpy((void*)sendbuf, (void*)&leave, sizeof(leave));

    iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
}

void Client::EventToServer(MoveEvent moveEvent)
{
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to send event to server: INVALID_SOCKET!\n");
        WSACleanup();
        return;
    }

    int iResult = 0;
    char sendbuf[sizeof(moveEvent)];
    moveEvent.event.head.id = ClientID;
    moveEvent.event.head.length = sizeof(moveEvent);
    moveEvent.event.head.seq_no = SequenceNumber;
    moveEvent.event.head.type = Event;

    memcpy((void*)sendbuf, (void*)&moveEvent, sizeof(moveEvent));

    iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
    SequenceNumber++;
}



void Client::TcpListen()
{

    // Receive data until the server closes the connection

    char RemainingMessagePart[DEFAULT_BUFLEN*2];
    int tBytesLeft = 0;

    bool bRemain = false;
    bool bWholeMessage = true;
    do {
        int iResult = 0;
        char recvbuf[DEFAULT_BUFLEN];

        char FirstMessagePart[DEFAULT_BUFLEN*2];
        char SecondMessagePart[DEFAULT_BUFLEN];
        char ThirdMessagePart[DEFAULT_BUFLEN];
        bool bFirstPart = true;
        bool bSecondPart = true;
        int SecondLen = 1;
        int ThirdLen = 1;

        int recvbuflen = DEFAULT_BUFLEN;
        if(ConnectSocket == INVALID_SOCKET)
        {
            using namespace std::chrono_literals;
            auto start = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(1s);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end-start;
            continue;
        }
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

        if (iResult > 0)
        {

            printf("Bytes received: %d\n", iResult);
            MsgHead* msghead;
            msghead = (MsgHead*)recvbuf;
            int BytesLeft = iResult + tBytesLeft;
            bWholeMessage = true;
            SecondLen = iResult - msghead->length;
            while(bWholeMessage)
            {
                if(BytesLeft < msghead->length && !bRemain)
                {
                    if(!bFirstPart)msghead = (MsgHead*)ThirdMessagePart;
                    else if(bFirstPart)msghead = (MsgHead*)SecondMessagePart;


                    printf("Header Length of last thing: %d\n", msghead->length);
                    printf("Bytes Left: %d\n", BytesLeft);
                    memcpy((void*)RemainingMessagePart,(void*)msghead , BytesLeft);;

                    bRemain = true;
                    tBytesLeft = BytesLeft;
                    break;
                }

                if(bSecondPart && !bFirstPart)
                {
                    msghead = (MsgHead*)SecondMessagePart;
                    ThirdLen = SecondLen - msghead->length;
                    memcpy((void*)FirstMessagePart, (void*)&SecondMessagePart , msghead->length);
                    memcpy((void*)ThirdMessagePart, (void*)((char*)&SecondMessagePart + msghead->length) , SecondLen - msghead->length);

                    bSecondPart = false;

                }else if(!bFirstPart)
                {
                    msghead = (MsgHead*)ThirdMessagePart;
                    SecondLen = ThirdLen - msghead->length;
                    memcpy((void*)FirstMessagePart, (void*)&ThirdMessagePart , msghead->length);
                    memcpy((void*)SecondMessagePart, (void*)((char*)&ThirdMessagePart + msghead->length) , ThirdLen - msghead->length);

                    bSecondPart = true;
                }

                if(bRemain)
                {
                    msghead = (MsgHead*)FirstMessagePart;

                    char* temp = ((char*)&FirstMessagePart + tBytesLeft);
                    memcpy((void*)FirstMessagePart, (void*)&RemainingMessagePart, tBytesLeft);
                    memcpy((void*)((char*)&FirstMessagePart + tBytesLeft), (void*)&recvbuf, msghead->length - tBytesLeft);
                    memcpy((void*)SecondMessagePart, (void*)((char*)&recvbuf + msghead->length - tBytesLeft) , iResult - msghead->length + tBytesLeft);
                    SecondLen = iResult - msghead->length + tBytesLeft;
                    bFirstPart = false;
                    bRemain = false;
                    tBytesLeft = 0;
                }

                if(bFirstPart)
                {
                    memcpy((void*)FirstMessagePart, (void*)&recvbuf , msghead->length);
                    memcpy((void*)SecondMessagePart, (void*)((char*)&recvbuf + msghead->length) , iResult - msghead->length);
                    SecondLen = iResult - msghead->length;
                    bFirstPart = false;
                }

                printf("Second length: %d\n", SecondLen);
                printf("Third length: %d\n", ThirdLen);

                msghead = (MsgHead*)FirstMessagePart;

                printf("First Length: %d\n", msghead->length);
                printf("First ID: %d\n", msghead->id);
                printf("Sequence: %d\n", ServerSequenceNumber);
                BytesLeft -= msghead->length;



                NewPlayerMsg* msgplayer;
                msgplayer = (NewPlayerMsg*) FirstMessagePart;

                PlayerLeaveMsg* msgplayerleave;
                msgplayerleave = (PlayerLeaveMsg*)FirstMessagePart;

                NewPlayerPositionMsg* msgplayerposition;
                msgplayerposition = (NewPlayerPositionMsg*)FirstMessagePart;

                if(ClientID == 0 && msghead->type == Join)
                {
                    printf("Recieved MsgHead Join... \n");
                    ClientID = msghead->id;
                    printf("New Client ID: %d\n", ClientID);
                    ServerSequenceNumber++;
                    continue;
                }
                else if(msgplayer->msg.type == NewPlayer)
                {
                    printf("Recieved NewPlayer... \n");

                    world->GetActors()->push_back(Actor());
                    world->GetActors()->at(world->GetActors()->capacity() - 1).SetID(msgplayer->msg.head.id);
                    world->SetChanged();

                    ServerSequenceNumber++;
                    continue;

                }else if(msgplayer->msg.type == NewPlayerPosition)
                {
                    bool First = false;
                    printf("Recieved NewPlayerPosition... \n");
                    int id = msgplayerposition->msg.head.id;
                    std::vector<Actor>* actors = world->GetActors();
                    ServerSequenceNumber++;
                    for(int i = 0; i < actors->capacity(); i++)
                    {
                        if(actors->at(i).GetID() == id)
                        {
                            actors->at(i).SetLocation(msgplayerposition->pos.x, msgplayerposition->pos.y);
                            world->SetChanged();
                            printf("Setting position and updating ... \n");
                            break;
                        }
                    }
                    continue;
                }else if(msgplayer->msg.type == PlayerLeave)
                {
                    printf("Recieved PlayerLeave... \n");
                    int id = msgplayerleave->msg.head.id;
                    std::vector<Actor>* actors = world->GetActors();
                    ServerSequenceNumber++;
                    for(int i = 0; i < actors->capacity(); i++)
                    {
                        if(actors->at(i).GetID() == id)
                        actors->erase(actors->begin() +  i);
                        world->SetChanged();
                    }
                    continue;
                }
            }
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
        {
            printf("recv failed: %d\n", WSAGetLastError());
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
    } while (true);
}

void Client::StartThread()
{
    std::thread* t1 = new std::thread(&Client::TcpListen, this);
}

