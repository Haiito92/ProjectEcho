#pragma once
#include "UStateMachine.generated.h"

class UIdle;
class UWalk;
class URun;
class UJump;
class UFall;
class UWallRun;
class UDeath;
class URevive;
class URewind;


class UInputDataConfig;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle,
	Walk,
	Run,
	Fall,
	Jump,
	WallRun,
	Death,
	Revive,
	Rewind
};

UCLASS()
class UStateMachine : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void InitStates(ACharacterST* InCharacter);
	UFUNCTION()
	void StartState(const EState& newState);
	UFUNCTION()
	void AddState(UState* newState,const EState& nameState);
	UFUNCTION()
	void ChangeState(const EState& newState);
	
	void Tick(float DeltaTime);
	
	UPROPERTY()
	UIdle* Idle;
	UPROPERTY()
	UWalk* Walk;
	UPROPERTY()
	URun* Run;
	UPROPERTY()
	UJump* Jump;
	UPROPERTY()
	UFall* Fall;
	UPROPERTY()
	UWallRun* WallRun;
	UPROPERTY()
	UDeath* Death;
	UPROPERTY()
	URevive* Revive;
	UPROPERTY()
	URewind* Rewind;
	
protected:

	UPROPERTY()
	UState* CurrentState;
	UPROPERTY()
	UState* PreviousState;

	UPROPERTY()
	TMap<EState,UState*> StateMap;
};
