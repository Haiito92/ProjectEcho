#pragma once
#include "InteractableMechanic/InteractorComponent.h"
#include "StateMachine/UStateMachine.h"
#include "UState.generated.h"

class UReflectComponent;
class URecordHandlerComponent;
class ACharacterST;
class UStateMachine;
class UGrabbingComponent;
class URecordManagerSubsystem;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EStateSettings: uint8
{
	None = 0,
	CanGrab = 1 << 0,
	CanInteract = 1 << 1,
	CanRecord = 1 << 2,
	CanPropulse = 1 << 3,
	CanReflect = 1 << 4,
	All = 31
};

ENUM_CLASS_FLAGS(EStateSettings);

UCLASS(Blueprintable)
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
	EState EnumState = EState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask))
	EStateSettings StateSettings = EStateSettings::All;
	
protected:
	
	UFUNCTION()
	bool CanUseGrab();
	
	UFUNCTION()
	bool CanUseRecord();
	
	UFUNCTION()
	bool CanUseInteract();
	
	UFUNCTION()
	bool CanUsePropulse();
	
	UFUNCTION()
	bool CanUseReflect();

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
	void OnPropulse();
	
	UFUNCTION()
	void OnReflect();
	
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
	
	UPROPERTY()
	TObjectPtr<UReflectComponent> ReflectComponent;
};