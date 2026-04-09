#pragma once
#include "StateMachine/UState.h"
#include "UFallRelease.generated.h"
UCLASS()
class UFallRelease : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnMovePressed(FVector2D InMoveInput);
};
