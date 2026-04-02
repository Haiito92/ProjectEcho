#pragma once
#include "StateMachine/UStateMachine.h"
#include "StateMachine/States/UFall.h"
#include "StateMachine/States/UWallRun.h"
#include "StateMachine/States/URun.h"
#include "StateMachine/States/UJump.h"
#include "StateMachine/States/UIdle.h"
#include "StateMachine/States/UWalk.h"
#include "UPlayerStateMachine.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UPlayerStateMachine : public UStateMachine
{
	GENERATED_BODY()
public:
	virtual void InitStates() override;
	
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
	UPROPERTY()
	UIdle* Idle;
	UPROPERTY()
	UWalk* Walk;
	UPROPERTY()
	URun* Run ;
	UPROPERTY()
	UJump* Jump;
	UPROPERTY()
	UFall* Fall;
	UPROPERTY()
	UWallRun* WallRun;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* AJump;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* AWalk;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* ALook;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* ARun;
	
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToRun;
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToWalk;
};
