#include "Public/StateMachine/UState.h"

#include "GameFramework/CharacterMovementComponent.h"
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
}

void UState::Enter()
{
	Character->OnGrabbingStarted.AddDynamic(this, &UState::OnGrabbingStarted);
	Character->OnThrowingStarted.AddDynamic(this, &UState::OnThrowingStarted);
}

void UState::Tick(float DeltaTime)
{
}

void UState::Exit()
{
	Character->OnGrabbingStarted.RemoveDynamic(this, &UState::OnGrabbingStarted);
	Character->OnThrowingStarted.RemoveDynamic(this, &UState::OnThrowingStarted);
}

bool UState::CanUseGrab()
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
