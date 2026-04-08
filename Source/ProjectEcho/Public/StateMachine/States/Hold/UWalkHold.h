#pragma once
#include "StateMachine/UState.h"
#include "UWalkHold.generated.h"

UCLASS()
class UWalkHold : public UState
{
public:
	GENERATED_BODY()
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
private:
	UFUNCTION()
	void OnMoveReleased(bool IsReleased);
	
	UFUNCTION()
	void OnRunningStarted(bool InRunningStarted);
	
	UFUNCTION()
	void OnMovePressed(FVector2D InMoveInput);
	
	UFUNCTION()
	void OnThrowingStarted();
};
