#pragma once
#include "Data/UStateMachineSettings.h"
#include "UStateMachine.generated.h"


class UInputDataConfig;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EState : uint8
{
	None,
	Idle,
	Move,
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
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeState,EState,newState);
	UPROPERTY(BlueprintAssignable)
	FOnChangeState OnChangeState;
	
protected:

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UState* CurrentState;
	UPROPERTY()
	UState* PreviousState;
	
	
	UPROPERTY()
	UStateMachineSettings* CurrentStateMachineSettings;
	UPROPERTY()
	TMap<EState,UState*> StateMap;
};
