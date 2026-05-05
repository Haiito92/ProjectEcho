#include "Public/StateMachine/UState.h"

#include "EchoSystem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrabMechanic/GrabbingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordHandlerComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "ReflectMechanic/ReflectComponent.h"
#include "StateMachine/ACharacterST.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

class UStateMachine;

UState::UState()
{
}

void UState::InitState(UStateMachine* InStateMachine,ACharacterST* InCharacter)
{
	StateMachine = InStateMachine;
	Character = InCharacter;
	GrabbingComponent = Character->FindComponentByClass<UGrabbingComponent>();
	RecordManagerSubsystem = GetWorld()->GetSubsystem<URecordManagerSubsystem>();
	InteractorComponent = Character->FindComponentByClass<UInteractorComponent>();
	ReflectComponent = Character->FindComponentByClass<UReflectComponent>();
	PropulseComponent = Character -> FindComponentByClass<UPropulseComponent>();
	RecordHandlerComponent = Character->RecordHandlerComponent;
}

void UState::Enter()
{
	Character->OnGrabbingStarted.AddDynamic(this, &UState::OnGrabbingStarted);
	Character->OnThrowingStarted.AddDynamic(this, &UState::OnThrowingStarted);
	Character->OnRecord.AddDynamic(this, &UState::OnRecord);
	Character->OnDestroySlot.AddDynamic(this, &UState::OnDestroySlot);
	Character->OnIncrementSlot.AddDynamic(this, &UState::OnIncrementSlot);
	Character->OnDecrementSlot.AddDynamic(this, &UState::OnDecrementSlot);
	Character->OnDeath.AddDynamic(this, &UState::OnDeath);
	Character->OnInteract.AddDynamic(this, &UState::OnInteract);
	Character->OnRevive.AddDynamic(this, &UState::OnRevive);
	Character->OnStartPropulse.AddDynamic(this, &UState::OnPropulseInputStarted);
	Character->OnStopPropulse.AddDynamic(this, &UState::OnPropulseInputStopped);
	Character->OnPropulsed.AddDynamic(this, &UState::OnPropulsed);
	Character->OnReflectInputStarted.AddDynamic(this, &UState::OnReflectInputStarted);
	Character->OnReflectInputCompleted.AddDynamic(this, &UState::OnReflectInputCompleted);
	Character->OnReflected.AddDynamic(this, &UState::OnReflected);
	RecordManagerSubsystem->OnStartPlayerRewinding.AddDynamic(this, &UState::OnRewindingStarted);
	RecordManagerSubsystem->OnStopPlayerRewinding.AddDynamic(this, &UState::OnRewindingEnded);
	
	Character->bCanBeReflected = (StateSettings & EStateSettings::CanBeReflected) == EStateSettings::CanBeReflected;
	Character->bCanBePropulsed = (StateSettings & EStateSettings::CanBePropulsed) == EStateSettings::CanBePropulsed;
}

void UState::Tick(float DeltaTime)
{
	if (CanUseReflect() && IsValid(ReflectComponent) && ReflectComponent->IsOn())
	{
		ReflectComponent->SetCastStartLocation(Character->FirstPersonCameraComponent->GetComponentLocation());
		ReflectComponent->SetCastDirection(UKismetMathLibrary::GetForwardVector(Character->GetControlRotation()));
	}
}

void UState::Exit()
{
	Character->OnGrabbingStarted.RemoveDynamic(this, &UState::OnGrabbingStarted);
	Character->OnThrowingStarted.RemoveDynamic(this, &UState::OnThrowingStarted);
	Character->OnRecord.RemoveDynamic(this, &UState::OnRecord);
	Character->OnDestroySlot.RemoveDynamic(this, &UState::OnDestroySlot);
	Character->OnIncrementSlot.RemoveDynamic(this, &UState::OnIncrementSlot);
	Character->OnDecrementSlot.RemoveDynamic(this, &UState::OnDecrementSlot);
	Character->OnDeath.RemoveDynamic(this, &UState::OnDeath);
	Character->OnInteract.RemoveDynamic(this, &UState::OnInteract);
	Character->OnRevive.RemoveDynamic(this, &UState::OnRevive);
	Character->OnStartPropulse.RemoveDynamic(this, &UState::OnPropulseInputStarted);
	Character->OnStopPropulse.RemoveDynamic(this, &UState::OnPropulseInputStopped);
	Character->OnPropulsed.RemoveDynamic(this, &UState::OnPropulsed);
	Character->OnReflectInputStarted.RemoveDynamic(this, &UState::OnReflectInputStarted);
	Character->OnReflectInputCompleted.RemoveDynamic(this, &UState::OnReflectInputCompleted);
	Character->OnReflected.RemoveDynamic(this, &UState::OnReflected);
	RecordManagerSubsystem->OnStartPlayerRewinding.RemoveDynamic(this, &UState::OnRewindingStarted);
	RecordManagerSubsystem->OnStopPlayerRewinding.RemoveDynamic(this, &UState::OnRewindingEnded);
	
	Character->bCanBeReflected = false;
	Character->bCanBePropulsed = false;
}

bool UState::CanUseGrab()
{
	return (StateSettings & EStateSettings::CanGrab) == EStateSettings::CanGrab;
}

bool UState::CanUseRecord()
{
	return (StateSettings & EStateSettings::CanRecord) == EStateSettings::CanRecord;
}

bool UState::CanUseInteract()
{
	return (StateSettings & EStateSettings::CanInteract) == EStateSettings::CanInteract;
}

bool UState::CanUsePropulse()
{
	return (StateSettings & EStateSettings::CanPropulse) == EStateSettings::CanPropulse;
}

bool UState::CanUseReflect()
{
	return (StateSettings & EStateSettings::CanReflect) == EStateSettings::CanReflect;
}

void UState::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UState::OnGrabbingStarted()
{
	if (CanUseGrab())
	{
		if (GrabbingComponent->IsGrabbing())
		{
			if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::TryRelease), FRecordedAction(ERecordedAction::ForceGrab));
			
			if (GrabbingComponent->TryRelease())
				Character->OnValidRelease.Broadcast();
		}
		else
		{
			if (GrabbingComponent->TryGrab(Character->GetControlRotation()))
				Character->OnValidGrab.Broadcast();
			
			if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::TryGrab), FRecordedAction(ERecordedAction::ForceRelease));
		}
	}
}

void UState::OnThrowingStarted()
{
	if (CanUseGrab())
	{
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::TryThrow), FRecordedAction(ERecordedAction::ForceGrab));
		
		if (GrabbingComponent->TryThrow(Character->GetControlRotation()))
			Character->OnValidThrow.Broadcast();
	}
}

void UState::OnRewindingStarted()
{
	Character->OnEndRecord.Broadcast();
	StateMachine->ChangeState(EState::Rewind);
}

void UState::OnRewindingEnded()
{
	StateMachine->ChangeState(EState::Idle);
}

void UState::CheckIsFalling() const
{
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::Fall);
}

void UState::OnJumpingStarted()
{
	StateMachine->ChangeState(EState::Jump);
}

void UState::OnMoveReleased()
{
	StateMachine->ChangeState(EState::Idle);
}

void UState::OnRecord()
{
	if (CanUseRecord())
	{
		if (RecordManagerSubsystem->IsRecording())
			RecordManagerSubsystem->StopRecord();
		else
		{
			Character->OnStartRecord.Broadcast();
			
			//Actions Played only on first Replay
			TArray<FRecordedAction> RestoreStateActions;
			
			//Actions Played at the beginning of each replay
			TArray<FRecordedAction> FirstActions;
			
			//Force Grab to Restore Grab State
			if (GrabbingComponent->IsGrabbing()) RestoreStateActions.Add(FRecordedAction(ERecordedAction::ForceGrab));
			if (IsValid(ReflectComponent) && ReflectComponent->IsOn()) FirstActions.Add(FRecordedAction(ERecordedAction::StartReflect));
			
			RecordManagerSubsystem->StartRecord(Character, RestoreStateActions, FirstActions);
		}
			
	}
}

void UState::OnIncrementSlot()
{
	if (CanUseRecord())
		RecordManagerSubsystem->IncrementSelectedSlot();
}

void UState::OnDecrementSlot()
{
	if (CanUseRecord())
		RecordManagerSubsystem->DecrementSelectedSlot();
}

void UState::OnDestroySlot()
{
	if (CanUseRecord())
		RecordManagerSubsystem->DestroySelectedTimeline();
}

void UState::OnDeath()
{
	StateMachine->ChangeState(EState::Death);
}

void UState::OnInteract()
{
	if (CanUseInteract() && !GrabbingComponent->IsGrabbing() && IsValid(InteractorComponent))
	{
		IInteractor::Execute_TryInteract(
			InteractorComponent,
			Character->FirstPersonCameraComponent->GetComponentLocation(),
			UKismetMathLibrary::GetForwardVector(Character->GetControlRotation())
			);
		
		if (IsValid(RecordHandlerComponent))
		{
			UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "Valid Record Handler", FColor::Green, 3.0f);
			RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::Interact));
		}
	}
}

void UState::OnPropulseInputStarted()
{
	if (CanUsePropulse() && IsValid(PropulseComponent))
	{
		PropulseComponent->StartPropulse();
		
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::StartPropulse));
	}
}

void UState::OnPropulseInputStopped()
{
	if (CanUsePropulse() && IsValid(PropulseComponent))
	{
		PropulseComponent->StopPropulse();
		
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::StopPropulse));
	}
}

void UState::OnPropulsed(const FVector& PropulseDirection, float PropulsePower)
{
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "UState: On Propulsed", FColor::Green, 3.0f);

	FVector PropulseForce = PropulseDirection.GetSafeNormal() * PropulsePower;
	Character->LaunchCharacter(PropulseForce, false, false);
}

void UState::OnReflectInputStarted()
{
	if (CanUseReflect() && IsValid(ReflectComponent))
	{
		ReflectComponent->StartReflect(
			Character->FirstPersonCameraComponent->GetComponentLocation(),
			UKismetMathLibrary::GetForwardVector(Character->GetControlRotation())
			);
		
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::StartReflect));
	}
}

void UState::OnReflectInputCompleted()
{
	if (CanUseReflect() && IsValid(ReflectComponent))
	{
		ReflectComponent->StopReflect();
		
		if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::StopReflect));
	}
}

void UState::OnReflected(const FVector& ReflectDirection, float ReflectPower)
{
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "UState: On Reflected", FColor::Green, 3.0f);

	FVector ReflectForce = ReflectDirection.GetSafeNormal() * ReflectPower;
	Character->LaunchCharacter(ReflectForce, false, false);
}

void UState::OnRevive()
{
}
