#pragma once
#include "StateMachine/UStateMachine.h"
#include "StateMachine/States/UFall.h"
#include "StateMachine/States/UWallRun.h"
#include "StateMachine/States/URun.h"
#include "StateMachine/States/UJump.h"
#include "StateMachine/States/UIdle.h"
#include "StateMachine/States/UWalk.h"
#include "UPlayerStateMachine.generated.h"

class UInputDataConfig;
class UInputMappingContext;
class APlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UPlayerStateMachine : public UStateMachine
{
	GENERATED_BODY()
public:
	virtual void InitStates() override;
	
	
	//UFUNCTION(BlueprintCallable)
	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
	UFUNCTION()
	void AIdle(const FInputActionValue& Value);
	UFUNCTION()
	void AMove(const FInputActionValue& Value);
	UFUNCTION()
	void ASprint(const FInputActionValue& Value);
	UFUNCTION()
	void AJump(const FInputActionValue& Value);
	
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
	
	
	
	UPROPERTY()
	APlayerController* PlayerController;
	
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToRun;
	UPROPERTY(EditDefaultsOnly,  Category = "State Machine")
	float MinimumSpeedToWalk;
};
