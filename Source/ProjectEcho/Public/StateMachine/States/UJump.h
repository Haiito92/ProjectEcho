#pragma once
#include "StateMachine/UState.h"
#include "UJump.generated.h"

UCLASS()
class UJump : public UState
{
	GENERATED_BODY()
public:
	
	virtual void InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int JumpForce = 10;
};
