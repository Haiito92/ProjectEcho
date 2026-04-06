#pragma once
#include "StateMachine/UState.h"
#include "URun.generated.h"

UCLASS()
class URun : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnRunning(bool InRunning);
	
private:
	bool IsRunning;
};
