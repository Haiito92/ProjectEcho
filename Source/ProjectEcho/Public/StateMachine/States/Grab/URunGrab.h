#pragma once
#include "StateMachine/UState.h"
#include "URunGrab.generated.h"

UCLASS()
class URunGrab : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnMoving(FVector2D MoveInput);
	
	UFUNCTION()
	void OnRunningReleased(bool InRunning);
	UFUNCTION()
	void OnMovingReleased(bool InRunning);
};
