#pragma once
#include "StateMachine/UState.h"
#include "UFall.generated.h"
UCLASS()
class UFall : public UState
{
	GENERATED_BODY()
public:
	UFall();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UPROPERTY()
	float BrakingDecelerationFalling = 1500.f;
	
	UPROPERTY()
	float AirControl = 0.5f;
	
	
};
