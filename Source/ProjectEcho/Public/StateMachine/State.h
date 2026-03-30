#pragma once

class State
{
public:
	virtual void Enter();
	virtual void Tick(float DeltaTime);
	virtual void Exit();
};
