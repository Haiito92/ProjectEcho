#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACharacterST.generated.h"

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
class PROJECTECHO_API ACharacterST : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterST();
	virtual void BeginPlay() override;
public:
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
	void ARun(const FInputActionValue& Value);
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
	void PlayerTakeDamage(int value);
	
	UFUNCTION(BlueprintCallable)
	void Kill();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	
	UFUNCTION(BlueprintCallable)
	void Revive();
	
	UFUNCTION()
	void ActivateCharacterInput();
	
	UFUNCTION()
	void DeactivateCharacterInput();
	
	UFUNCTION()
	void InitStateMachine();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovePressed, FVector2D, MoveInputVector);
	FMovePressed OnMovePressed;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveStarted, bool, isPress);
	FMoveStarted OnMoveStarted;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveReleased);
	FMoveReleased OnMoveReleased;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpStarted);
	FJumpStarted OnJumpingStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRunPressed);
	FRunPressed OnRunning;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunStarted, bool, isRunning);
	FRunStarted OnRunningStarted;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunReleased, bool, isRunning);
	FRunReleased OnRunningReleased;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabStarted);
	FGrabStarted OnGrabbingStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReleaseStarted);
	FReleaseStarted OnReleaseStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowStarted);
	FThrowStarted OnThrowingStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecord);
	FOnRecord OnRecord;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroySlot);
	FOnDestroySlot OnDestroySlot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncrementSlot);
	FOnIncrementSlot OnIncrementSlot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDecrementSlot);
	FOnDecrementSlot OnDecrementSlot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	FOnDeath OnDeath;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEnd);
	FOnDeathEnd OnDeathEnd;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRevive);
	FOnRevive OnRevive;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
	FOnRevive OnInteract;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidGrab);
	UPROPERTY(BlueprintAssignable)
	FOnValidGrab OnValidGrab;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidThrow);
	UPROPERTY(BlueprintAssignable)
	FOnValidThrow OnValidThrow;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidRelease);
	UPROPERTY(BlueprintAssignable)
	FOnValidRelease OnValidRelease;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataConfig* InputActions;
	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputMappingContext* InputMapping;
	
	UPROPERTY()
	float WalkSpeed = 600.f;
	UPROPERTY()
	float RunSpeed = 900.f;
	UPROPERTY()
	int Life = 100;
	UPROPERTY()
	bool IsRunInputOn = false;
	
	
	UPROPERTY()
	FVector2D MoveInputDir = FVector2D::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine")
	UStateMachine* StateMachine;
	
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
};
