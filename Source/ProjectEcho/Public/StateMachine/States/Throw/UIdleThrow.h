#pragma once
#include "StateMachine/UState.h"
#include "UIdleThrow.generated.h"

UCLASS()
class UIdleThrow : public UState
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
