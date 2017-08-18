#include "world.h"
#include "Actors/actor.h"
#include "gamepanel.h"
#include "ui_mainwindow.h"
#include "thread"
#include <iostream>

World::World()
{
}

std::vector<Actor>* World::GetActors()
{
    return &Actors; 
}



void World::UpdateUi(Ui::MainWindow *ui)
{
    if(ui != NULL)
    {
        QMetaObject::invokeMethod(Ui->widget, "renderGame", Qt::QueuedConnection);
    }
}

void World::SetUi(Ui::MainWindow *ui)
{
    Ui = ui;
    Ui->widget->SetActor(&Actors);
}

Ui::MainWindow* World::GetUi()
{
    return Ui;
}

void World::SetChanged()
{
    Changed = true;
}

Actor* World::GetPlayerWithID(int id)
{
    for(int i = 0; i < Actors.capacity(); i++)
    {
        if(Actors.at(i).GetID() == id) return &Actors.at(i);
    }
    return NULL;
}

void World::WorldTick()
{
    while(true)
    {
        using namespace std::chrono_literals;
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(17));
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end-start;
        if(!Changed) continue;
        Changed = false;
        UpdateUi(Ui);
    }
}

void World::StartThread()
{
    std::thread* t2 = new std::thread(&World::WorldTick, this);
}
