#pragma once
#include "StateMachine/UState.h"
#include "URevive.generated.h"

UCLASS()
class URevive : public UState
{
	GENERATED_BODY()
public:
	URevive();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	

};
