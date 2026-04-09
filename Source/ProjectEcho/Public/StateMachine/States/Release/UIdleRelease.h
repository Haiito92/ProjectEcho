#pragma once
#include "StateMachine/UState.h"
#include "UIdleRelease.generated.h"

UCLASS()
class UIdleRelease : public UState
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
	void OnjumpingStarted();
};
