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
class UJumpHold;
class UFallHold;

class UIdleThrow;
class UWalkThrow;
class URunThrow;
class UJumpThrow;
class UFallThrow;

class UIdleGrab;
class UWalkGrab;
class URunGrab;
class UJumpGrab;
class UFallGrab;

class UIdleRelease;
class UWalkRelease;
class URunRelease;
class UJumpRelease;
class UFallRelease;


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
	
	IdleGrab,
	WalkGrab,
	RunGrab,
	FallGrab,
	JumpGrab,
	
	IdleHold,
	WalkHold,
	RunHold,
	FallHold,
	JumpHold,
	
	IdleThrow,
	WalkThrow,
	RunThrow,
	FallThrow,
	JumpThrow,
	
	IdleRelease,
	WalkRelease,
	RunRelease,
	FallRelease,
	JumpRelease
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
	UJumpGrab* JumpGrab;
	UPROPERTY()
	UFallGrab* FallGrab;
	
	UPROPERTY()
	UIdleThrow* IdleThrow;
	UPROPERTY()
	UWalkThrow* WalkThrow;
	UPROPERTY()
	URunThrow* RunThrow;
	UPROPERTY()
	UJumpThrow* JumpThrow;
	UPROPERTY()
	UFallThrow* FallThrow;
	
	UPROPERTY()
	UIdleHold* IdleHold;
	UPROPERTY()
	UWalkHold* WalkHold;
	UPROPERTY()
	URunHold* RunHold;
	UPROPERTY()
	UJumpHold* JumpHold;
	UPROPERTY()
	UFallHold* FallHold;
	
	UPROPERTY()
	UIdleRelease* IdleRelease;
	UPROPERTY()
	UWalkRelease* WalkRelease;
	UPROPERTY()
	URunRelease* RunRelease;
	UPROPERTY()
	UJumpRelease* JumpRelease;
	UPROPERTY()
	UFallRelease* FallRelease;
	
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
