#include "gamepanel.h"
#include "qstyle.h"
#include "qstyleoption.h"
#include "qpainter.h"
#include "Actors/actor.h"
#include "mainwindow.h"
#include "QMouseEvent"
#include <iostream>

GamePanel::GamePanel(QWidget *parent) : QWidget(parent)
{
}

void GamePanel::paintEvent(QPaintEvent *)
{

    QPalette pal = this->palette();
    if(this->backgroundRole() != Qt::white)
    pal.setColor(this->backgroundRole(), Qt::white);

    this->setPalette(pal);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(Actors != NULL)
    for(int i = 0; i < Actors->capacity(); i++)
    {
       if(Actors->at(i).GetID() == clientID)
       {
            painter.setPen(Qt::blue);
       }else painter.setPen(Qt::black);

       if(Actors->at(i).GetID() > 0)
       painter.drawRect(Actors->at(i).GetLocation()->at(0) + 100, Actors->at(i).GetLocation()->at(1) + 100,20,20);

    }
}


void GamePanel::SetActor(std::vector<Actor> *actors)
{
    Actors = actors;
}

void GamePanel::SetClientID(int id)
{
    clientID = id;
}

void GamePanel::mousePressEvent(QMouseEvent *event)
{
    MoveEvent move;
    move.pos.x = event->x()-100;
    move.pos.y = event->y()-100;

    client->EventToServer(move);
}

Client* GamePanel::GetClient()
{
    return client;
}

void GamePanel::SetClient(Client *client)
{
    this->client = client;
}

void GamePanel::renderGame()
{
    update();
}

