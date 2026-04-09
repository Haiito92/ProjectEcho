#pragma once
#include "StateMachine/UState.h"
#include "URunHold.generated.h"

UCLASS()
class URunHold : public UState
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
	UFUNCTION()
	void OnThrowingStarted();
	UFUNCTION()
	void OnReleaseStarted();
	UFUNCTION()
	void OnjumpingStarted();
};
