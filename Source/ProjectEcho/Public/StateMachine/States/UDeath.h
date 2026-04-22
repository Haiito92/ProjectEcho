#pragma once
#include "StateMachine/UState.h"
#include "UDeath.generated.h"

UCLASS()
class UDeath : public UState
{
	GENERATED_BODY()
public:
	UDeath();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	
protected:
	
	virtual void OnRevive() override;

	
};
