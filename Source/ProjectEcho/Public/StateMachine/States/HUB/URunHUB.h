#pragma once
#include "StateMachine/UState.h"
#include "URunHUB.generated.h"

UCLASS()
class URunHUB : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
};
