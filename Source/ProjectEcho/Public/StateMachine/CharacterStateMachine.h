#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStateMachine.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputDataConfig;
class UInputMappingContext;
class UPlayerStateMachine;

UCLASS()
class PROJECTECHO_API ACharacterStateMachine : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterStateMachine();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputDataConfig* InputActions;
	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UPlayerStateMachine* StateMachine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
};
