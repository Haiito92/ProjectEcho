#pragma once
#include "StateMachine/UState.h"
#include "UFall.generated.h"
UCLASS()
class UFall : public UState
{
	GENERATED_BODY()
public:
	virtual void InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter) override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float BrakingDecelerationFalling = 1500.f;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float AirControl = 0.5f;
	
	
};
