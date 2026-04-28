#pragma once
#include "StateMachine/UState.h"
#include "UJump.generated.h"

UCLASS()
class UJump : public UState
{
	GENERATED_BODY()
public:
	UJump();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UPROPERTY()
	int JumpForce = 10;
};
