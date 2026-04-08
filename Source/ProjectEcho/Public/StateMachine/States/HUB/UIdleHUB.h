#pragma once
#include "StateMachine/UState.h"
#include "UIdleHUB.generated.h"

UCLASS()
class UIdleHUB : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
};
