#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Actors/actor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

Client* MainWindow::GetClient()
{
    return client;

}

void MainWindow::SetClient(Client *client)
{
    this->client = client;
    ui->widget->SetClient(client);
}

Ui::MainWindow* MainWindow::GetUI()
{
    return ui;
}



void MainWindow::on_pushButton_clicked()
{
    client->Connect(ui->lineEditIP->text().toStdString(), std::stoi(ui->lineEditPort->text().toStdString()));
}

void MainWindow::on_pushButton_2_clicked()
{
    client->LeaveServer();
}

