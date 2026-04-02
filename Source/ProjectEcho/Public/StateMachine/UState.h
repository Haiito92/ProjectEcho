#pragma once
#include "UState.generated.h"

class UPlayerStateMachine;

UCLASS()
class UState : public UObject
{
public:
	GENERATED_BODY()
	UState();
	
	void InitStateMachine(UPlayerStateMachine *sm);
	virtual void Enter();
	virtual void Tick(float DeltaTime);
	virtual void Exit();
protected:
	UPROPERTY()
	UPlayerStateMachine* StateMachine;
};