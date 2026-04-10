#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GrabMechanic/GrabbingComponent.h"
#include "ACharacterST.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputDataConfig;
class UInputMappingContext;
class UStateMachine;

UCLASS()
class PROJECTECHO_API ACharacterST : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterST();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void InitPlayer();
	
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
	void TakeDamage(int value);
	
	UFUNCTION(BlueprintCallable)
	void Kill();
	
	UFUNCTION(BlueprintCallable)
	void Revive();
	
	
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
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRevive);
	FOnRevive OnRevive;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataConfig* InputActions;
	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RunSpeed = 900.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int NumberSlot = 5;
	
	UPROPERTY()
	bool IsRunInputOn = false;
	
	UPROPERTY(BlueprintReadWrite)
	int Life = 100;
	
	UPROPERTY()
	FVector2D MoveInputDir = FVector2D::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UStateMachine* StateMachine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
};
