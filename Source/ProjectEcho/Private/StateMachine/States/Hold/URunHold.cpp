#include "StateMachine/States/Hold/URunHold.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URunHold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallHold);
}

void URunHold::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URunHold::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URunHold::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URunHold::OnRunningReleased);
	Character->OnThrowingStarted.AddDynamic(this,&URunHold::OnThrowingStarted);
	Character->OnGrabbingStarted.AddDynamic(this,&URunHold::OnReleaseStarted);
	Character->OnJumpingStarted.AddDynamic(this,&URunHold::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
}

void URunHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URunHold::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URunHold::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URunHold::OnRunningReleased);
	Character->OnThrowingStarted.RemoveDynamic(this,&URunHold::OnThrowingStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&URunHold::OnReleaseStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&URunHold::OnjumpingStarted);
}

void URunHold::OnMoving(FVector2D MoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * MoveInput.Y + Character->GetActorRightVector() * MoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void URunHold::OnRunningReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::WalkHold);
}

void URunHold::OnMovingReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::IdleHold);
}

void URunHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::RunThrow);
}

void URunHold::OnReleaseStarted()
{
	StateMachine->ChangeState(EState::RunRelease);
}

void URunHold::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpHold);
}

