#pragma once
#include "StateMachine/ACharacterST.h"

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "StateMachine/UStateMachine.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GrabMechanic/GrabbingComponent.h"
#include "RecordManager/RecordHandlerComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/UState.h"
#include "StateMachine/Data/UInputDataConfig.h"
#include "StateMachine/Data/UPlayerData.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"


class UPlayerData;
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
	
	FVector Vel = GetCharacterMovement()->Velocity;
	if (Vel.Size() > MaxVelocity)
	{
		UE_LOG(LogTemp,Warning, TEXT("Velocity size is greater than max velocity size"));
		Vel = Vel.GetSafeNormal() * MaxVelocity;
		GetCharacterMovement()->Velocity = Vel;
	}
}

void ACharacterST::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
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
	
	Input->BindAction(InputActions->AJump, ETriggerEvent::Started, this, &ACharacterST::AJump);
	
	Input->BindAction(InputActions->ALook, ETriggerEvent::Triggered, this, &ACharacterST::ALook);
	
	Input->BindAction(InputActions->AGrab, ETriggerEvent::Started, this, &ACharacterST::AGrabStarted);
	
	Input->BindAction(InputActions->AThrow, ETriggerEvent::Started, this, &ACharacterST::AThrowStarted);
	
	Input->BindAction(InputActions->AIncrementSlot, ETriggerEvent::Started, this, &ACharacterST::IncrementSlot);
	Input->BindAction(InputActions->ADecrementSlot, ETriggerEvent::Started, this, &ACharacterST::DecrementSlot);
	Input->BindAction(InputActions->ARecord, ETriggerEvent::Started, this, &ACharacterST::Record);
	Input->BindAction(InputActions->ADestroySlot, ETriggerEvent::Started, this, &ACharacterST::DestroySlot);
	Input->BindAction(InputActions->AInteract, ETriggerEvent::Started,this,&ACharacterST::AInteract);
	
	Input->BindAction(InputActions->APropulse, ETriggerEvent::Started,this,&ACharacterST::AStartPropulse);
	Input->BindAction(InputActions->APropulse, ETriggerEvent::Completed,this,&ACharacterST::AStopPropulse);
	
	Input->BindAction(InputActions->AReflect, ETriggerEvent::Started,this,&ACharacterST::AStartReflect);
	Input->BindAction(InputActions->AReflect, ETriggerEvent::Completed,this,&ACharacterST::AStopReflect);
}

void ACharacterST::InitPlayer()
{
	RecordHandlerComponent = FindComponentByClass<URecordHandlerComponent>();
	GrabbingComponent = FindComponentByClass<UGrabbingComponent>();
	InitStateMachine();
	LoadData();
}

void ACharacterST::LoadData()
{
	UPlayerData* playerData = GetDefault<UDataAssetDeveloperSettings>()->PlayerData.LoadSynchronous();
	Life = playerData->InitLife;
	MaxVelocity = playerData->MaxVelocity;
	GetCharacterMovement()->MaxAcceleration = playerData->MoveAcceleration;
	GetCharacterMovement()->AirControlBoostVelocityThreshold = playerData->AirPrecision;
	GetCharacterMovement()->GravityScale = playerData->GravityScale;
}

void ACharacterST::AMove(const FInputActionValue& Value)
{
	MoveInputDir = Value.Get<FVector2D>();
	OnMovePressed.Broadcast(MoveInputDir);
}

void ACharacterST::AMoveStarted(const FInputActionValue& Value)
{
	MoveInputDir = Value.Get<FVector2D>();
	OnMoveStarted.Broadcast(true);
}

void ACharacterST::AMoveReleased(const FInputActionValue& Value)
{
	MoveInputDir = Value.Get<FVector2D>();
	OnMoveReleased.Broadcast();
}


void ACharacterST::ARunStarted(const FInputActionValue& Value)
{
	IsRunInputOn = true;
	OnRunningStarted.Broadcast(IsRunInputOn);
}

void ACharacterST::ARunReleased(const FInputActionValue& Value)
{
	IsRunInputOn = false;
	OnRunningReleased.Broadcast(IsRunInputOn);
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

void ACharacterST::AInteract()
{
	OnInteract.Broadcast();
}

void ACharacterST::AStartPropulse()
{
	OnStartPropulse.Broadcast();
}

void ACharacterST::AStopPropulse()
{
	OnStopPropulse.Broadcast();
}

void ACharacterST::AStartReflect()
{
	OnReflectInputStarted.Broadcast();
}

void ACharacterST::AStopReflect()
{
	OnReflectInputCompleted.Broadcast();
}

void ACharacterST::PlayerTakeDamage(int value)
{
	Life = FMath::Max(Life-value,0);
	if (Life <= 0)
		OnDeath.Broadcast();
}

void ACharacterST::Kill_Implementation()
{
	IKillable::Kill_Implementation();
	
	OnDeath.Broadcast();
	Life = 0;
}

void ACharacterST::Laserize_Implementation()
{
	ILaserizable::Laserize_Implementation();
	
	Execute_Kill(this);
}

void ACharacterST::DeathEnd()
{
	OnDeathEnd.Broadcast();
}

void ACharacterST::Revive()
{
	OnRevive.Broadcast();
}



void ACharacterST::InitStateMachine()
{
	StateMachine = NewObject<UStateMachine>(this);
	StateMachine->InitStates(this);
}

bool ACharacterST::CanBeReflected_Implementation() const
{
	return bCanBeReflected;
}

void ACharacterST::PrepareReflect_Implementation(AActor* ActorDoingReflect)
{
}

void ACharacterST::Reflect_Implementation(const FVector& ReflectDirection, float ReflectPower)
{
	OnReflected.Broadcast(ReflectDirection, ReflectPower);
}

void ACharacterST::FinalizeReflect_Implementation()
{
}

TArray<FRecordedAction> ACharacterST::GetToRecordActions()
{
	if (IsValid(RecordHandlerComponent)) return RecordHandlerComponent->GetToRecordActions();
	return TArray<FRecordedAction>();
}

TArray<FRecordedAction> ACharacterST::GetToRecordRewindActions()
{
	if (IsValid(RecordHandlerComponent)) return RecordHandlerComponent->GetToRecordRewindActions();
	return TArray<FRecordedAction>();
}

void ACharacterST::HandleRewindActionKey_Implementation(const FRecordedAction& RewindAction)
{
	switch (RewindAction.ActionEnum)
	{
	case ERecordedAction::ForceRelease:
		if (IsValid(GrabbingComponent))
		{
			GrabbingComponent->ForceRelease();
		}
	default: 
		break;
	}
}

bool ACharacterST::CanBePropulsed_Implementation() const
{
	return bCanBePropulsed;
}

void ACharacterST::PreparePropulse_Implementation(AActor* PropulsingActor)
{
}

void ACharacterST::Propulse_Implementation(const FVector& PropulseDirection, float PropulsePower)
{
	OnPropulsed.Broadcast(PropulseDirection, PropulsePower);
}

void ACharacterST::FinalizePropulse_Implementation()
{
}
