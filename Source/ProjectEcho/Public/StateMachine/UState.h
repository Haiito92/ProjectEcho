#pragma once
#include "InteractableMechanic/InteractorComponent.h"
#include "StateMachine/UStateMachine.h"
#include "UState.generated.h"

class URecordHandlerComponent;
class ACharacterST;
class UStateMachine;
class UGrabbingComponent;
class URecordManagerSubsystem;

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
	virtual bool CanUseRecord();
	
	UFUNCTION()
	virtual bool CanUseInteract();

	UFUNCTION()
	virtual void OnMovePressed(FVector2D InMoveInput);
	
	UFUNCTION()
	void OnGrabbingStarted();
	
	UFUNCTION()
	void OnThrowingStarted();
	
	UFUNCTION()
	void OnRewindingStarted();
	
	UFUNCTION()
	void OnRewindingEnded();
	
	UFUNCTION()
	void OnJumpingStarted();
	
	UFUNCTION()
	void OnMoveReleased();
	
	UFUNCTION()
	void OnRecord();
	
	UFUNCTION()
	void OnIncrementSlot();
	
	UFUNCTION()
	void OnDecrementSlot();
	
	UFUNCTION()
	void OnDestroySlot();
	
	UFUNCTION()
	void OnDeath();
	
	UFUNCTION()
	void OnInteract();
	
	UFUNCTION()
	virtual void OnRevive();
	
	UFUNCTION()
	void CheckIsFalling() const;
	
	UPROPERTY()
	TObjectPtr<URecordManagerSubsystem> RecordManagerSubsystem;
	
	UPROPERTY()
	TObjectPtr<UStateMachine> StateMachine;
	
	UPROPERTY()
	TObjectPtr<ACharacterST> Character;
	
	UPROPERTY()
	TObjectPtr<UGrabbingComponent> GrabbingComponent;
	
	UPROPERTY()
	TObjectPtr<UInteractorComponent> InteractorComponent;

	UPROPERTY()
	TObjectPtr<URecordHandlerComponent> RecordHandlerComponent;
};