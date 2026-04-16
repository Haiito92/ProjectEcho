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
	TObjectPtr<UIdle> Idle;
	UPROPERTY()
	TObjectPtr<UWalk> Walk;
	UPROPERTY()
	TObjectPtr<URun> Run;
	UPROPERTY()
	TObjectPtr<UJump> Jump;
	UPROPERTY()
	TObjectPtr<UFall> Fall;
	UPROPERTY()
	TObjectPtr<UWallRun> WallRun;
	UPROPERTY()
	TObjectPtr<UDeath> Death;
	UPROPERTY()
	TObjectPtr<URevive> Revive;
	UPROPERTY()
	TObjectPtr<URewind> Rewind;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeState,EState,newState);
	UPROPERTY(BlueprintAssignable)
	FOnChangeState OnChangeState;
	
protected:

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UState* CurrentState;
	UPROPERTY()
	UState* PreviousState;

	UPROPERTY()
	TMap<EState,UState*> StateMap;
};
