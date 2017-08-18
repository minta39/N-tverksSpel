#include "stdafx.h"
#include "GameThread.h"
#include <thread>

GameThread::GameThread()
{
}


GameThread::~GameThread()
{
}

void GameThread::Start()
{
	std::thread* t1 = new std::thread(&GameThread::run, this);
}