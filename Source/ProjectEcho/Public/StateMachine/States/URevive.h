#pragma once
#include "StateMachine/UState.h"
#include "URevive.generated.h"

UCLASS()
class URevive : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	virtual bool CanUseGrab() override;
	virtual bool CanUseRecord() override;
	virtual bool CanUseInteract() override;
	
private:
};
