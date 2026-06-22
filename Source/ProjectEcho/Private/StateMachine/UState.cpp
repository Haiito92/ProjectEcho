#include "Public/StateMachine/UState.h"

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controls/EPlayerActionType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrabMechanic/GrabbingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordHandlerComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "ReflectMechanic/ReflectComponent.h"
#include "ReflectMechanic/ReflectMechanicSettings.h"
#include "StateMachine/ACharacterST.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

class UStateMachine;

UState::UState()
{
}

void UState::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	StateMachine = InStateMachine;
	Character = InCharacter;
	GrabbingComponent = Character->FindComponentByClass<UGrabbingComponent>();
	RecordManagerSubsystem = GetWorld()->GetSubsystem<URecordManagerSubsystem>();
	InteractorComponent = Character->FindComponentByClass<UInteractorComponent>();
	ReflectComponent = Character->FindComponentByClass<UReflectComponent>();
	RecordHandlerComponent = Character->RecordHandlerComponent;
}

void UState::Enter()
{
	Character->OnThrowingStarted.AddDynamic(this, &UState::OnThrowingStarted);
	Character->OnRecord.AddDynamic(this, &UState::OnRecord);
	Character->OnDestroySlot.AddDynamic(this, &UState::OnDestroySlot);
	Character->OnIncrementSlot.AddDynamic(this, &UState::OnIncrementSlot);
	Character->OnDecrementSlot.AddDynamic(this, &UState::OnDecrementSlot);
	Character->OnDeath.AddDynamic(this, &UState::OnDeath);
	Character->OnDeathInRecord.AddDynamic(this, &UState::OnDeathInRecord);
	Character->OnInteractOrGrab.AddDynamic(this, &UState::OnInteractOrGrab);
	Character->OnPropulsed.AddDynamic(this, &UState::OnPropulsed);
	Character->OnReflectInputStarted.AddDynamic(this, &UState::OnReflectInputStarted);
	Character->OnReflectInputCompleted.AddDynamic(this, &UState::OnReflectInputCompleted);
	Character->OnReflected.AddDynamic(this, &UState::OnReflected);
	Character->OnActionLocked.AddDynamic(this, &UState::OnActionLocked);
	Character->OnActionUnlocked.AddDynamic(this, &UState::OnActionUnlocked);
	RecordManagerSubsystem->OnStartPlayerRewinding.AddDynamic(this, &UState::OnRewindingStarted);

	Character->bCanBeReflected = (StateSettings & EStateSettings::CanBeReflected) == EStateSettings::CanBeReflected;
	Character->bCanBePropulsed = (StateSettings & EStateSettings::CanBePropulsed) == EStateSettings::CanBePropulsed;
	Character->bCanBeLaserized = (StateSettings & EStateSettings::CanBeLaserized) == EStateSettings::CanBeLaserized;
	
	
	if (Character->ConsumeShouldRestoreReflect())
	{
		OnReflectInputStarted();
	}
}

void UState::Tick(float DeltaTime)
{
	if (CanUseReflect() && IsValid(ReflectComponent) && ReflectComponent->IsOn())
	{
		ReflectComponent->SetCastStartLocation(Character->FirstPersonCameraComponent->GetComponentLocation());
		ReflectComponent->SetCastDirection(UKismetMathLibrary::GetForwardVector(Character->GetControlRotation()));
	}
}

void UState::OnDeathInRecord()
{
	StopRecord(true);
}

void UState::Exit()
{
	Character->OnThrowingStarted.RemoveDynamic(this, &UState::OnThrowingStarted);
	Character->OnRecord.RemoveDynamic(this, &UState::OnRecord);
	Character->OnDestroySlot.RemoveDynamic(this, &UState::OnDestroySlot);
	Character->OnIncrementSlot.RemoveDynamic(this, &UState::OnIncrementSlot);
	Character->OnDecrementSlot.RemoveDynamic(this, &UState::OnDecrementSlot);
	Character->OnDeath.RemoveDynamic(this, &UState::OnDeath);
	Character->OnDeathInRecord.RemoveDynamic(this, &UState::OnDeathInRecord);
	Character->OnInteractOrGrab.RemoveDynamic(this, &UState::OnInteractOrGrab);
	Character->OnPropulsed.RemoveDynamic(this, &UState::OnPropulsed);
	Character->OnReflectInputStarted.RemoveDynamic(this, &UState::OnReflectInputStarted);
	Character->OnReflectInputCompleted.RemoveDynamic(this, &UState::OnReflectInputCompleted);
	Character->OnReflected.RemoveDynamic(this, &UState::OnReflected);
	Character->OnActionLocked.RemoveDynamic(this, &UState::OnActionLocked);
	Character->OnActionUnlocked.RemoveDynamic(this, &UState::OnActionUnlocked);
	RecordManagerSubsystem->OnStartPlayerRewinding.RemoveDynamic(this, &UState::OnRewindingStarted);

	Character->bCanBeReflected = false;
	Character->bCanBePropulsed = false;
	Character->bCanBeLaserized = false;
}

bool UState::CanUseGrab()
{
	return (StateSettings & EStateSettings::CanGrab) == EStateSettings::CanGrab && !ReflectComponent->IsOn();
}

bool UState::CanUseRecord()
{
	return (StateSettings & EStateSettings::CanRecord) == EStateSettings::CanRecord;
}

bool UState::CanUseInteract()
{
	return (StateSettings & EStateSettings::CanInteract) == EStateSettings::CanInteract && !ReflectComponent->IsOn() && !GrabbingComponent->IsGrabbing();
}

bool UState::CanUsePropulse()
{
	return (StateSettings & EStateSettings::CanPropulse) == EStateSettings::CanPropulse && !ReflectComponent->IsOn() && !GrabbingComponent->IsGrabbing();
}

bool UState::CanUseReflect()
{
	return (StateSettings & EStateSettings::CanReflect) == EStateSettings::CanReflect && !GrabbingComponent->IsGrabbing();
}

void UState::OnMovePressed(FVector2D InMoveInput)
{
	if (Character->GetLockedActions()[EPlayerActionType::Move]) return;
	
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UState::OnGrabbingStarted()
{
	
}

void UState::OnThrowingStarted()
{
	if (!CanUseGrab() || Character->GetLockedActions()[EPlayerActionType::Throw]) return;
	
	if (GrabbingComponent->TryThrow(Character->GetControlRotation()))
	{
		Character->OnValidThrow.Broadcast();
		
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(
		FRecordedAction(ERecordedAction::TryThrow), FRecordedAction(ERecordedAction::ForceGrab));
	}
}

void UState::OnRewindingStarted()
{
	Character->OnEndRecord.Broadcast();
	StateMachine->ChangeState(EState::Rewind);
}

void UState::CheckIsFalling() const
{
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::Fall);
}

void UState::OnJumpingStarted()
{
	if (Character->GetLockedActions()[EPlayerActionType::Jump]) return;
	
	StateMachine->ChangeState(EState::Jump);
}

void UState::OnMoveReleased()
{
	if (Character->GetLockedActions()[EPlayerActionType::Move]) return;
	
	StateMachine->ChangeState(EState::Idle);
}

void UState::OnRecord()
{
	if (!CanUseRecord()) return;
	
	if (RecordManagerSubsystem->IsRecording())
		StopRecord();
	else
		StartRecord();
}

void UState::StartRecord()
{
	if (Character->GetLockedActions()[EPlayerActionType::Record]) return;
	
	Character->OnStartRecord.Broadcast();

	//Actions Played only on first Replay
	TArray<FRecordedAction> RestoreStateActions;

	//Actions Played at the beginning of each replay
	TArray<FRecordedAction> FirstActions;

	//Force Grab to Restore Grab State
	if (GrabbingComponent->IsGrabbing()) RestoreStateActions.Add(FRecordedAction(ERecordedAction::ForceGrab));
	if (IsValid(ReflectComponent) && ReflectComponent->IsOn())
	{
		Character->OnValidReflect.Broadcast();
		FirstActions.Add(
		FRecordedAction(ERecordedAction::StartReflect));
	}
		

	RecordManagerSubsystem->StartRecord(Character, RestoreStateActions, FirstActions);
}

void UState::StopRecord(bool bForceStop)
{
	if (Character->GetLockedActions()[EPlayerActionType::StopRecord]) return;
	RecordManagerSubsystem->StopRecord(bForceStop);
}

void UState::OnIncrementSlot()
{
	if (!CanUseRecord() || Character->GetLockedActions()[EPlayerActionType::SelectTimeline]) return;
	
	RecordManagerSubsystem->IncrementSelectedSlot();
}

void UState::OnDecrementSlot()
{
	if (!CanUseRecord() || Character->GetLockedActions()[EPlayerActionType::SelectTimeline]) return;
	
	RecordManagerSubsystem->DecrementSelectedSlot();
}

void UState::OnDestroySlot()
{
	if (!CanUseRecord() || Character->GetLockedActions()[EPlayerActionType::DeleteTimeline]) return;
	RecordManagerSubsystem->DestroySelectedTimeline();
}

void UState::OnDeath()
{
	StateMachine->ChangeState(EState::Death);
}

void UState::OnInteractOrGrab()
{
	bool TryGrabSucceed = false;
	
	if (TryGrab())
		TryGrabSucceed = true;
	else
		TryInteract();
	
	Character->TryGrabOrInteract(TryGrabSucceed);
	
}

bool UState::TryGrab()
{
	if (!CanUseGrab() || Character->GetLockedActions()[EPlayerActionType::GrabOrRelease]) return false;
	
	if (GrabbingComponent->IsGrabbing())
	{
		if (GrabbingComponent->TryRelease())
		{
			Character->OnValidRelease.Broadcast();
			
			if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(
			FRecordedAction(ERecordedAction::TryRelease), FRecordedAction(ERecordedAction::ForceGrab));
		}
		
		return true; // We return true no matter what, because if release fail, we still can't interact after
	}
	
	FGrabbingRules GrabbingRules = FGrabbingRules();
	GrabbingRules.CollisionChannelsToIgnore.Add(ECC_Pawn);
	
	bool TryGrab = GrabbingComponent->TryGrab(Character->GetControlRotation(), GrabbingRules); 
	
	if (TryGrab)
	{
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(
			FRecordedAction(ERecordedAction::TryGrab), FRecordedAction(ERecordedAction::ForceRelease));
	}
	
	return TryGrab;
}

bool UState::TryInteract()
{
	if (!CanUseInteract() || GrabbingComponent->IsGrabbing() || !IsValid(InteractorComponent) || Character->GetLockedActions()[EPlayerActionType::Interact]) return false;
	
	bool bTryInteract = IInteractor::Execute_TryInteract(
		InteractorComponent,
		Character->FirstPersonCameraComponent->GetComponentLocation(),
		UKismetMathLibrary::GetForwardVector(Character->GetControlRotation())
	);
	
	if (bTryInteract)
	{
		if (IsValid(RecordHandlerComponent))
			RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::Interact));
	}
	
	return bTryInteract;
}

void UState::OnPropulsed(const FVector& PropulseDirection, float PropulsePower)
{
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "UState: On Propulsed",
	                                    FColor::Green, 3.0f);

	FVector PropulseForce = PropulseDirection.GetSafeNormal() * PropulsePower;
	Character->LaunchCharacter(PropulseForce, false, false);
}

void UState::OnReflectInputStarted()
{
	if (!CanUseReflect() || !IsValid(ReflectComponent) || ReflectComponent->IsOn() || Character->GetLockedActions()[EPlayerActionType::Reflect]) return;
	
	ReflectComponent->StartReflect(
		Character->FirstPersonCameraComponent->GetComponentLocation(),
		UKismetMathLibrary::GetForwardVector(Character->GetControlRotation())
	);

	if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(
		FRecordedAction(ERecordedAction::StartReflect));
}

void UState::OnReflectInputCompleted()
{
	if (!CanUseReflect() || !IsValid(ReflectComponent) || !ReflectComponent->IsOn() || Character->GetLockedActions()[EPlayerActionType::Reflect]) return;
	
	ReflectComponent->StopReflect();

	if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(
		FRecordedAction(ERecordedAction::StopReflect));
}

void UState::OnActionLocked(const EPlayerActionType& PlayerAction)
{
	switch (PlayerAction)
	{
		case EPlayerActionType::Reflect:
			{
				if (ReflectComponent->IsOn())
				{
					ReflectComponent->StopReflect();
					Character->SetShouldRestoreReflect(true);
				}
				break;
			}
		default:
			{
				break;
			}
	}
}

void UState::OnActionUnlocked(const EPlayerActionType& PlayerAction)
{
	switch (PlayerAction)
	{
	case EPlayerActionType::Reflect:
		{
			if (ReflectComponent->IsOn()) ReflectComponent->StopReflect();
			break;
		}
	default:
		{
			break;
		}
	}
}

void UState::OnReflected(const FVector& ReflectDirection, float ReflectPower)
{
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "UState: On Reflected",
	                                    FColor::Green, 3.0f);

	UWorld* World = GetWorld();
	UReflectMechanicSettings* ReflectSettings = nullptr;
	
	const UDataAssetDeveloperSettings* DataAssetSettings = GetDefault<UDataAssetDeveloperSettings>();
	if (IsValid(DataAssetSettings))
	{
		ReflectSettings = DataAssetSettings->ReflectMechanicSettings.LoadSynchronous();
	}
	
	if (Character->GetCharacterMovement()->IsFalling() || !IsValid(World) || !IsValid(ReflectSettings))
	{
		FVector ReflectForce = ReflectDirection.GetSafeNormal() * ReflectPower;
		Character->LaunchCharacter(ReflectForce, true, true);
		return;
	}
	
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if (!IsValid(CapsuleComponent))
	{
		FVector ReflectForce = ReflectDirection.GetSafeNormal() * ReflectPower;
		Character->LaunchCharacter(ReflectForce, true, true);
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "No capsule comp: can't fake up vector." , FColor::Green, 3.0f);
		return;
	}
	
	FVector CapsuleDownVector = CapsuleComponent->GetUpVector() * -1;
	FVector LineStart = CapsuleComponent->GetComponentLocation() + CapsuleDownVector * CapsuleComponent->GetScaledCapsuleHalfHeight();
	FVector LineEnd = LineStart + CapsuleDownVector * 50.0f;
	
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(Character);
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.bTraceComplex = true;
	
	UEchoDebug::DrawLine(this, EEchoSystem::Reflect, LineStart, LineEnd, FColor::Red, 3.0f);
	
	FHitResult HitResult;
	World->LineTraceSingleByChannel(
		HitResult,
		LineStart, 
		LineEnd,
		ECollisionChannel::ECC_WorldStatic,
		QueryParams
		);
	
	if (!HitResult.bBlockingHit)
	{
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "No blocking hit: no need to fake up vector." , FColor::Green, 3.0f);
		FVector ReflectForce = ReflectDirection.GetSafeNormal() * ReflectPower;
		Character->LaunchCharacter(ReflectForce, true, true);
		return;
	}
	
	FVector ReflectDirectionNormalized = ReflectDirection.GetSafeNormal();
	FVector UpVector = HitResult.ImpactNormal;
	
	float Dot = UKismetMathLibrary::Dot_VectorVector(UpVector, ReflectDirectionNormalized);
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "Dot: " + FString::SanitizeFloat(Dot), FColor::Green, 3.0f);
	
	if (Dot > 0)
	{
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "Dot is positive: no need to fake up vector." , FColor::Green, 3.0f);
		FVector ReflectForce = ReflectDirection.GetSafeNormal() * ReflectPower;
		Character->LaunchCharacter(ReflectForce, true, true);
		return;
	}
	
	FVector RightVector = UKismetMathLibrary::Cross_VectorVector(UpVector, ReflectDirectionNormalized);
	FVector ForwardVector = UKismetMathLibrary::Cross_VectorVector(RightVector, UpVector);
	
	// We don't divide by length product cuz it's 1 since both vectors are normalized
	float Cos = UKismetMathLibrary::Dot_VectorVector(ForwardVector.GetSafeNormal(), ReflectDirectionNormalized);
	
	float ReflectAngleToGround = FMath::RadiansToDegrees(FMath::Acos(Cos));
	
	if (ReflectAngleToGround > ReflectSettings->ReflectLiftAngle)
	{
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "ReflectAngleToGround superior to threshold: no need to fake up vector." , FColor::Green, 3.0f);
		FVector ReflectForce = ReflectDirection.GetSafeNormal() * ReflectPower;
		Character->LaunchCharacter(ReflectForce, true, true);
		return;
	}
	
	FVector ReflectForce = UKismetMathLibrary::RotateAngleAxis(ForwardVector, -ReflectSettings->ReflectLiftAngle, RightVector) * ReflectPower;
	Character->LaunchCharacter(ReflectForce, true, true);
}
