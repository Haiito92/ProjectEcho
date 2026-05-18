#pragma once
#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "GameFramework/Character.h"
#include "GrabMechanic/GrabberActorInterface.h"
#include "KillMechanic/Killable.h"
#include "LaserMechanic/Laserizable.h"
#include "PropulseMechanic/Propulsable.h"
#include "ReflectMechanic/Reflectable.h"
#include "RecordManager/RecordHandlerInterface.h"
#include "ACharacterST.generated.h"

class UGrabbingComponent;
class URecordHandlerComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputDataConfig;
class UInputMappingContext;
class UStateMachine;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class PROJECTECHO_API ACharacterST : public ACharacter, public IKillable, public IReflectable, public IPropulsable, public IRecordHandlerInterface, public IPlayerInterface, public ILaserizable, public IGrabberActorInterface
{
	GENERATED_BODY()

public:
	ACharacterST();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void InitPlayer();
	
	UFUNCTION()
	void LoadData();
	
	UFUNCTION()
	void AMove(const FInputActionValue& Value);
	UFUNCTION()
	void AMoveStarted(const FInputActionValue& Value);
	UFUNCTION()
	void AMoveReleased(const FInputActionValue& Value);
	
	UFUNCTION()
	void ARunStarted(const FInputActionValue& Value);
	UFUNCTION()
	void ARunReleased(const FInputActionValue& Value);
	
	UFUNCTION()
	void AJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void ALook(const FInputActionValue& Value);
	
	UFUNCTION()
	void AGrabStarted(const FInputActionValue& Value);
	
	UFUNCTION()
	void AThrowStarted(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void IncrementSlot();
	
	UFUNCTION(BlueprintCallable)
	void DecrementSlot();
	
	UFUNCTION(BlueprintCallable)
	void DestroySlot();
	
	UFUNCTION(BlueprintCallable)
	void Record();
	
	UFUNCTION(BlueprintCallable)
	void AInteract();
	
	UFUNCTION(BlueprintCallable)
	void AStartPropulse();
	
	UFUNCTION(BlueprintCallable)
	void AStopPropulse();
	
	UFUNCTION(BlueprintCallable)
	void AStartReflect();
	
	UFUNCTION(BlueprintCallable)
	void AStopReflect();
	
	UFUNCTION(BlueprintCallable)
	void PlayerTakeDamage(int value);
	
	UFUNCTION(BlueprintCallable)
	virtual void Kill_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Laserize_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	
	UFUNCTION(BlueprintCallable)
	void Revive();
	
	UFUNCTION()
	void InitStateMachine();
	
	UFUNCTION()
	virtual bool CanBeReflected_Implementation() const override;
	
	UFUNCTION()
	virtual void PrepareReflect_Implementation(AActor* ActorDoingReflect) override;
	
	UFUNCTION()
	virtual void Reflect_Implementation(const FVector& ReflectDirection, float ReflectPower) override;
	
	UFUNCTION()
	virtual void FinalizeReflect_Implementation() override;
	
	UFUNCTION()
	virtual TArray<FRecordedAction> GetToRecordActions() override;
	
	UFUNCTION()
	virtual TArray<FRecordedAction> GetToRecordRewindActions() override;
	
	UFUNCTION()
	virtual void HandleRewindActionKey_Implementation(const FRecordedAction& RewindAction) override;

	UFUNCTION()
	virtual bool CanBePropulsed_Implementation() const override;
	
	UFUNCTION()
	virtual void PreparePropulse_Implementation(AActor* PropulsingActor) override;
	
	UFUNCTION()
	virtual void Propulse_Implementation(const FVector& PropulseDirection, float PropulsePower) override;
	
	UFUNCTION()
	virtual void FinalizePropulse_Implementation() override;
	
	virtual void ForceRelease_Implementation() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovePressed, FVector2D, MoveInputVector);
	UPROPERTY(BlueprintAssignable)
	FMovePressed OnMovePressed;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveStarted, bool, isPress);
	UPROPERTY(BlueprintAssignable)
	FMoveStarted OnMoveStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveReleased);
	UPROPERTY(BlueprintAssignable)
	FMoveReleased OnMoveReleased;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpStarted);
	UPROPERTY(BlueprintAssignable)
	FJumpStarted OnJumpingStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRunPressed);
	UPROPERTY(BlueprintAssignable)
	FRunPressed OnRunning;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunStarted, bool, isRunning);
	UPROPERTY(BlueprintAssignable)
	FRunStarted OnRunningStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunReleased, bool, isRunning);
	UPROPERTY(BlueprintAssignable)
	FRunReleased OnRunningReleased;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabStarted);
	UPROPERTY(BlueprintAssignable)
	FGrabStarted OnGrabbingStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReleaseStarted);
	UPROPERTY(BlueprintAssignable)
	FReleaseStarted OnReleaseStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowStarted);
	UPROPERTY(BlueprintAssignable)
	FThrowStarted OnThrowingStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecord);
	UPROPERTY(BlueprintAssignable)
	FOnRecord OnRecord;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroySlot);
	UPROPERTY(BlueprintAssignable)
	FOnDestroySlot OnDestroySlot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncrementSlot);
	UPROPERTY(BlueprintAssignable)
	FOnIncrementSlot OnIncrementSlot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDecrementSlot);
	UPROPERTY(BlueprintAssignable)
	FOnDecrementSlot OnDecrementSlot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEnd);
	UPROPERTY(BlueprintAssignable)
	FOnDeathEnd OnDeathEnd;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRevive);
	UPROPERTY(BlueprintAssignable)
	FOnRevive OnRevive;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
	UPROPERTY(BlueprintAssignable)
	FOnRevive OnInteract;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartPropulse);
	UPROPERTY(BlueprintAssignable)
	FOnStartPropulse OnStartPropulse;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopPropulse);
	UPROPERTY(BlueprintAssignable)
	FOnStopPropulse OnStopPropulse;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPropulsed, const FVector&, PropulseDirection, float, PropulsePower);
	UPROPERTY(BlueprintAssignable)
	FOnPropulsed OnPropulsed;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReflectInputStarted);
	UPROPERTY(BlueprintAssignable)
	FOnReflectInputStarted OnReflectInputStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReflectInputCompleted);
	UPROPERTY(BlueprintAssignable)
	FOnReflectInputCompleted OnReflectInputCompleted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReflected, const FVector&, ReflectDirection, float, ReflectPower);
	UPROPERTY(BlueprintAssignable)
	FOnReflected OnReflected;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidGrab);
	UPROPERTY(BlueprintAssignable)
	FOnValidGrab OnValidGrab;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidThrow);
	UPROPERTY(BlueprintAssignable)
	FOnValidThrow OnValidThrow;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidRelease);
	UPROPERTY(BlueprintAssignable)
	FOnValidRelease OnValidRelease;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRecord);
	UPROPERTY(BlueprintAssignable)
	FOnStartRecord OnStartRecord;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndRecord);
	UPROPERTY(BlueprintAssignable)
	FOnEndRecord OnEndRecord;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputDataConfig> InputActions;
	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	
	UPROPERTY()
	int Life = 100;
	UPROPERTY()
	bool IsRunInputOn = false;
	UPROPERTY()
	float MaxVelocity = 900.f;
	
	
	UPROPERTY()
	FVector2D MoveInputDir = FVector2D::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine")
	TObjectPtr<UStateMachine> StateMachine;
	
	UPROPERTY()
	TObjectPtr<URecordHandlerComponent> RecordHandlerComponent;
	
	UPROPERTY()
	TObjectPtr<UGrabbingComponent> GrabbingComponent;
	
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;
	
	bool bCanBeReflected = false;
	bool bCanBePropulsed = false;
};
