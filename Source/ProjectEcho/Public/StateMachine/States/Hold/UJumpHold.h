#pragma once
#include "StateMachine/UState.h"
#include "UJumpHold.generated.h"

UCLASS()
class UJumpHold : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnMovePressed(FVector2D InMoveInput);
	
	UFUNCTION()
	void OnThrowingStarted();
	
	UFUNCTION()
	void OnReleaseStarted();
};
