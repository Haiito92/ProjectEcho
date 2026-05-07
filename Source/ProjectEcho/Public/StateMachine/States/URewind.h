#pragma once
#include "StateMachine/UState.h"
#include "URewind.generated.h"

UCLASS()
class URewind : public UState
{
	GENERATED_BODY()
public:
	
	virtual void InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UFUNCTION()
	void OnRewindingEnded();
};
