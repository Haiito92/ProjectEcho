#include "Public/StateMachine/UState.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/ACharacterST.h"

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
}

void UState::Tick(float DeltaTime)
{
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
}

bool UState::CanUseGrab()
{
	return true;
}

bool UState::CanUseRecord()
{
	return true;
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
			GrabbingComponent->TryRelease();
		else
			GrabbingComponent->TryGrab(Character->GetControlRotation());
	}
}

void UState::OnThrowingStarted()
{
	if (CanUseGrab())
		GrabbingComponent->TryThrow(Character->GetControlRotation());
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
			RecordManagerSubsystem->StartRecord(Character);
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

void UState::OnRevive()
{
}
