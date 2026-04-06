#pragma once
#include "UStateMachine.generated.h"

class UIdle;
class UMove;
class URun;
class UJump;
class UFall;
class UWallRun;


class UInputDataConfig;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle,
	Move,
	Sprint,
	Jump,
	Fall,
	WallRun
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
	UMove* Move;
	UPROPERTY()
	URun* Run ;
	UPROPERTY()
	UJump* Jump;
	UPROPERTY()
	UFall* Fall;
	UPROPERTY()
	UWallRun* WallRun;
	
	
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToRun;
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToWalk;
	
protected:

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UState* CurrentState;
	UPROPERTY()
	UState* PreviousState;

	UPROPERTY()
	TMap<EState,UState*> StateMap;
};
