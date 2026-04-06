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
	
	UFUNCTION()
	void AMove(const FInputActionValue& Value);
	
	UFUNCTION()
	void AMoveReleased(const FInputActionValue& Value);
	UFUNCTION()
	void ARun(const FInputActionValue& Value);
	UFUNCTION()
	void AJump(const FInputActionValue& Value);
	UFUNCTION()
	void ALook(const FInputActionValue& Value);
	
	UFUNCTION()
	void InitStateMachine();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveInput, FVector2D, MoveInputVector);
	FMoveInput OnMoveInput;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveReleased, bool, isReleased);
	FMoveReleased OnMoveReleased;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJumpInput, bool, isJumping);
	FJumpInput OnJumping;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunInput, bool, isRunning);
	FRunInput OnRunning;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataConfig* InputActions;
	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 600.f;
	UPROPERTY(EditAnywhere)
	float RunSpeed = 900.f;
	
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UStateMachine* StateMachine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
};
