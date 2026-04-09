#include "StateMachine/States/Hold/UJumpHold.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UJumpHold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallHold);
}

void UJumpHold::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UJumpHold::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UJumpHold::OnMovePressed);
	Character->OnThrowingStarted.AddDynamic(this,&UJumpHold::OnThrowingStarted);
	Character->OnGrabbingStarted.AddDynamic(this,&UJumpHold::OnReleaseStarted);
	Character->Jump();
}

void UJumpHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UJumpHold::OnMovePressed);
	Character->OnThrowingStarted.RemoveDynamic(this,&UJumpHold::OnThrowingStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UJumpHold::OnReleaseStarted);
}

void UJumpHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::JumpThrow);
}

void UJumpHold::OnReleaseStarted()
{
	StateMachine->ChangeState(EState::JumpRelease);
}
