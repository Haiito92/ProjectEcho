#pragma once
#include "UStateMachine.generated.h"

class UIdle;
class UWalk;
class URun;
class UJump;
class UFall;
class UWallRun;

class UIdleHold;
class UWalkHold;
class URunHold;

class UIdleThrow;
class UWalkThrow;
class URunThrow;

class UIdleGrab;
class UWalkGrab;
class URunGrab;


class UInputDataConfig;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle,
	Walk,
	Run,
	Jump,
	Fall,
	WallRun,
	IdleGrab,
	WalkGrab,
	RunGrab,
	IdleHold,
	WalkHold,
	RunHold,
	IdleThrow,
	WalkThrow,
	RunThrow,
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
	UIdleGrab* IdleGrab;
	UPROPERTY()
	UWalkGrab* WalkGrab;
	UPROPERTY()
	URunGrab* RunGrab;
	
	UPROPERTY()
	UIdleThrow* IdleThrow;
	UPROPERTY()
	UWalkThrow* WalkThrow;
	UPROPERTY()
	URunThrow* RunThrow;
	
	UPROPERTY()
	UIdleHold* IdleHold;
	UPROPERTY()
	UWalkHold* WalkHold;
	UPROPERTY()
	URunHold* RunHold;
	
	
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToRun;
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToWalk;
	
protected:

	UPROPERTY()
	UState* CurrentState;
	UPROPERTY()
	UState* PreviousState;

	UPROPERTY()
	TMap<EState,UState*> StateMap;
};
