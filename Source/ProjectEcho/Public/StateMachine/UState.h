#pragma once
#include "ACharacterST.h"
#include "InteractableMechanic/InteractorComponent.h"
#include "ReflectMechanic/Reflectable.h"
#include "PropulseMechanic/PropulseComponent.h"
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
	CanBePropulsed = 1 << 4,
	CanReflect = 1 << 5,
	CanBeReflected = 1 << 6,
	CanBeLaserized = 1 << 7,
	All = 0b11111111
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
	
	virtual void InitState(UStateMachine *InStateMachine,ACharacterST* InCharacter);
	
	UPROPERTY()
	EState EnumState = EState::None;
	
	UPROPERTY(meta = (Bitmask))
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
	void OnJumpingStarted();
	
	UFUNCTION()
	void OnMoveReleased();
	
	UFUNCTION()
	void OnRecord();
	
	UFUNCTION()
	void StartRecord();
	
	UFUNCTION()
	void StopRecord(bool bForceStop = false);
	
	UFUNCTION()
	void OnIncrementSlot();
	
	UFUNCTION()
	void OnDecrementSlot();
	
	UFUNCTION()
	void OnDestroySlot();
	
	UFUNCTION()
	void OnDeath();
	
	UFUNCTION()
	void OnDeathInRecord();
	
	UFUNCTION()
	void OnInteractOrGrab();
	UFUNCTION()
	bool TryGrab();
	UFUNCTION()
	bool TryInteract();
	
	UFUNCTION(meta=(DeprecatedFunction))
	void OnPropulsed(const FVector& PropulseDirection, float PropulsePower);
	
	UFUNCTION()
	void OnReflected(const FVector& ReflectDirection, float ReflectPower);
	
	UFUNCTION()
	void OnReflectInputStarted();
	
	UFUNCTION()
	void OnReflectInputCompleted();
	
	UFUNCTION()
	void OnActionLocked(const EPlayerActionType& PlayerAction);
	UFUNCTION()
	void OnActionUnlocked(const EPlayerActionType& PlayerAction);
	
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