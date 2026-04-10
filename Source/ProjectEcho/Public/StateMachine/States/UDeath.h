#pragma once
#include "StateMachine/UState.h"
#include "UDeath.generated.h"

UCLASS()
class UDeath : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
protected:
	virtual bool CanUseGrab() override;
	virtual bool CanUseRecord() override;
	
	virtual void OnRevive() override;

	
};
