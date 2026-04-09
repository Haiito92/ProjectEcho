#pragma once
#include "StateMachine/UStateMachine.h"
#include "UState.generated.h"

class ACharacterST;
class UStateMachine;
class UGrabbingComponent;

UCLASS()
class UState : public UObject
{
public:
	GENERATED_BODY()
	UState();
	
	virtual void Enter();
	virtual void Tick(float DeltaTime);
	virtual void Exit();	
	
	void InitState(UStateMachine *InStateMachine,ACharacterST* InCharacter);
	
	UPROPERTY()
	EState EnumState;
	
protected:
	
	UFUNCTION()
	virtual bool CanUseGrab();

	UFUNCTION()
	virtual void OnMovePressed(FVector2D InMoveInput);
	
	UFUNCTION()
	void OnGrabbingStarted();
	
	UFUNCTION()
	void OnThrowingStarted();
	
	UFUNCTION()
	void OnJumpingStarted();
	
	UFUNCTION()
	void OnMoveReleased();
	
	UFUNCTION()
	void CheckIsFalling() const;
	
	
	UPROPERTY()
	UStateMachine* StateMachine;
	
	UPROPERTY()
	TObjectPtr<ACharacterST> Character;
	
	UPROPERTY()
	UGrabbingComponent* GrabbingComponent;
};