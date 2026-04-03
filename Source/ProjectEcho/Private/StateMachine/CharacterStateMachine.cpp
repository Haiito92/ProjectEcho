#pragma once
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/CharacterStateMachine.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "StateMachine/InputDataConfig.h"
#include "StateMachine/UPlayerStateMachine.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ACharacterStateMachine::ACharacterStateMachine()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;
	
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);
	
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}


void ACharacterStateMachine::BeginPlay()
{
	Super::BeginPlay();
	StateMachine = GetComponentByClass<UPlayerStateMachine>();
	if(StateMachine == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("state machine null"));
		return;
	}
}


void ACharacterStateMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void ACharacterStateMachine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr) return;
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr) return;
	
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	    
	if(InputActions == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("input action null"));
		return;
	}
	
	Input->BindAction(InputActions->AMove,ETriggerEvent::Completed ,StateMachine,&UPlayerStateMachine::AIdle);
	Input->BindAction(InputActions->AMove,ETriggerEvent::Canceled ,StateMachine,&UPlayerStateMachine::AIdle);
	Input->BindAction(InputActions->AMove, ETriggerEvent::Triggered, StateMachine, &UPlayerStateMachine::AMove);
	Input->BindAction(InputActions->ASprint, ETriggerEvent::Started, StateMachine, &UPlayerStateMachine::ASprint);
	Input->BindAction(InputActions->AJump, ETriggerEvent::Started, StateMachine, &UPlayerStateMachine::AJump);
}