#pragma once
#include "StateMachine/UState.h"
#include "UIdleHold.generated.h"

UCLASS()
class UIdleHold : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
private:
	UFUNCTION()
	void OnMovePressed(FVector2D dir);
	UFUNCTION()
	void OnThrowingStarted();
	UFUNCTION()
	void OnReleaseStarted();
	UFUNCTION()
	void OnjumpingStarted();
};
