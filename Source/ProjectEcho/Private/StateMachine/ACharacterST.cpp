#pragma once
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GrabMechanic/GrabbingComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/InputDataConfig.h"



class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ACharacterST::ACharacterST()
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


void ACharacterST::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterST::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine == nullptr)
		return;
	StateMachine->Tick(DeltaTime);
}

void ACharacterST::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr) return;
	
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	
	if (PlayerController == nullptr) return;
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if(InputActions == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Data Input Missing on Character"));
		return;
	}
	
	Input->BindAction(InputActions->AMove, ETriggerEvent::Triggered, this, &ACharacterST::AMove);
	Input->BindAction(InputActions->AMove, ETriggerEvent::Started, this, &ACharacterST::AMoveStarted);
	Input->BindAction(InputActions->AMove, ETriggerEvent::Completed, this, &ACharacterST::AMoveReleased);
	
	Input->BindAction(InputActions->ARun, ETriggerEvent::Triggered, this, &ACharacterST::ARun);
	Input->BindAction(InputActions->ARun, ETriggerEvent::Started, this, &ACharacterST::ARunStarted);
	Input->BindAction(InputActions->ARun, ETriggerEvent::Completed, this, &ACharacterST::ARunReleased);
	
	Input->BindAction(InputActions->AJump, ETriggerEvent::Started, this, &ACharacterST::AJump);
	
	Input->BindAction(InputActions->ALook, ETriggerEvent::Triggered, this, &ACharacterST::ALook);
	
	Input->BindAction(InputActions->AGrab, ETriggerEvent::Started, this, &ACharacterST::AGrabStarted);
	
	Input->BindAction(InputActions->AThrow, ETriggerEvent::Started, this, &ACharacterST::AThrowStarted);
	
	Input->BindAction(InputActions->AIncrementSlot, ETriggerEvent::Started, this, &ACharacterST::IncrementSlot);
	Input->BindAction(InputActions->ADecrementSlot, ETriggerEvent::Started, this, &ACharacterST::DecrementSlot);
	Input->BindAction(InputActions->ARegister, ETriggerEvent::Started, this, &ACharacterST::Record);
	Input->BindAction(InputActions->ADestroySlot, ETriggerEvent::Started, this, &ACharacterST::DestroySlot);
}

void ACharacterST::InitPlayer()
{
	InitStateMachine();
}

void ACharacterST::AMove(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	OnMovePressed.Broadcast(Input);
}

void ACharacterST::AMoveStarted(const FInputActionValue& Value)
{
	OnMoveStarted.Broadcast(true);
}

void ACharacterST::AMoveReleased(const FInputActionValue& Value)
{
	OnMoveReleased.Broadcast();
}

void ACharacterST::ARun(const FInputActionValue& Value)
{
	bool bRunning = Value.Get<bool>();
	OnRunning.Broadcast(bRunning);
}

void ACharacterST::ARunStarted(const FInputActionValue& Value)
{
	OnRunningStarted.Broadcast(true);
}

void ACharacterST::ARunReleased(const FInputActionValue& Value)
{
	OnRunningReleased.Broadcast(true);
}

void ACharacterST::AJump(const FInputActionValue& Value)
{
	OnJumpingStarted.Broadcast();
}

void ACharacterST::ALook(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(-Input.Y);
}

void ACharacterST::AGrabStarted(const FInputActionValue& Value)
{
	OnReleaseStarted.Broadcast();
	OnGrabbingStarted.Broadcast();
}

void ACharacterST::AThrowStarted(const FInputActionValue& Value)
{
	OnThrowingStarted.Broadcast();
}

void ACharacterST::IncrementSlot()
{
	OnIncrementSlot.Broadcast();
}

void ACharacterST::DecrementSlot()
{
	OnDecrementSlot.Broadcast();
}

void ACharacterST::DestroySlot()
{
	OnDestroySlot.Broadcast();
}

void ACharacterST::Record()
{
	OnRecord.Broadcast();
}

void ACharacterST::InitStateMachine()
{
	StateMachine = NewObject<UStateMachine>(this);
	StateMachine->InitStates(this);
}
