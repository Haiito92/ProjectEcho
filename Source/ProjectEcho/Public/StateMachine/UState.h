#pragma once
#include "StateMachine/UStateMachine.h"
#include "UState.generated.h"

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
	virtual void OnRevive();
	
	UFUNCTION()
	void CheckIsFalling() const;
	
	UPROPERTY()
	URecordManagerSubsystem* RecordManagerSubsystem;
	
	UPROPERTY()
	UStateMachine* StateMachine;
	
	UPROPERTY()
	TObjectPtr<ACharacterST> Character;
	
	UPROPERTY()
	UGrabbingComponent* GrabbingComponent;
};