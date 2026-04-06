#pragma once
#include "UState.generated.h"

class ACharacterST;
class UStateMachine;

UCLASS()
class UState : public UObject
{
public:
	GENERATED_BODY()
	UState();
	
	void InitStateMachine(UStateMachine *InStateMachine,ACharacterST* InCharacter);
	virtual void Enter();
	virtual void Tick(float DeltaTime);
	virtual void Exit();
protected:
	UPROPERTY()
	UStateMachine* StateMachine;
	UPROPERTY()
	TObjectPtr<ACharacterST> Character;
};