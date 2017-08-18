#include "mainwindow.h"
#include <QApplication>

#include <stdio.h>
#include "Client/client.h"
#include "Data/world.h"
#include "ui_mainwindow.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

    World *world;
    Client *client;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    world = new World();
    client = new Client();

    w.SetClient(client);
    client->SetWorld(world);
    world->SetUi(w.GetUI());

    client->StartThread();
    world->StartThread();


    return a.exec();
}

