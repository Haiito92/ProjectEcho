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
	void OnMoveStarted(bool IsPressed);
	UFUNCTION()
	void OnThrowingStarted();
};
