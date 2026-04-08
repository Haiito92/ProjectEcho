#pragma once
#include "StateMachine/UState.h"
#include "UWalkHUB.generated.h"

UCLASS()
class UWalkHUB : public UState
{
public:
	GENERATED_BODY()
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
};
