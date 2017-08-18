#pragma once
class GameThread
{
public:
	GameThread();
	~GameThread();

	void Start();

private:
	virtual void run() = 0;
};

