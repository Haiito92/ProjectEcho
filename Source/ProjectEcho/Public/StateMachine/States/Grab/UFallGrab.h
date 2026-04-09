#pragma once
#include "StateMachine/UState.h"
#include "UFallGrab.generated.h"
UCLASS()
class UFallGrab : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnMovePressed(FVector2D InMoveInput);
};
