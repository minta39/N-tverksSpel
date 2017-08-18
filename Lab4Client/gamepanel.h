#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QWidget>
#include "Client/client.h"

class Actor;

class GamePanel : public QWidget
{
    Q_OBJECT
private:
    std::vector<Actor>* Actors = NULL;
    int clientID = 0;
    Client *client = NULL;
public:
    explicit GamePanel(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    void SetClient(Client *client);
    Client* GetClient();

    void SetActor(std::vector<Actor>* actors);

    void SetClientID(int id);
protected:
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:
    void renderGame();
};

#endif // GAMEPANEL_H
