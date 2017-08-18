#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "Actors/actor.h"
#include <thread>
#include <QWidget>

#define MAXY 100
#define MINY -100
#define MAXX 100
#define MINX -100

namespace Ui {
class MainWindow;
}
class World
{
private:
    std::vector<Actor> Actors;
    Ui::MainWindow *Ui = NULL;
    bool Changed = false;
public:
    World();
    std::vector<Actor>* GetActors();
    void Start();
    void WorldThread();

    void SetChanged();

    void SetUi(Ui::MainWindow *ui);
    Ui::MainWindow* GetUi();
    void UpdateUi(Ui::MainWindow *ui);
    Actor* GetPlayerWithID(int id);

    void WorldTick();

    void StartThread();
};

#endif // WORLD_H
