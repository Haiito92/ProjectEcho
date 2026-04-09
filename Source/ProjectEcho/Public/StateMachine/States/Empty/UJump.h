#pragma once
#include "StateMachine/UState.h"
#include "UJump.generated.h"

UCLASS()
class UJump : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnMovePressed(FVector2D InMoveInput);
};
