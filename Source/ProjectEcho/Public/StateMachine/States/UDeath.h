#pragma once
#include "StateMachine/UState.h"
#include "UDeath.generated.h"

UCLASS()
class UDeath : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter) override;
	
protected:
	UFUNCTION()
	virtual void OnRevive();

	
};
