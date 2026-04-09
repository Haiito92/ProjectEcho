#include "StateMachine/States/Empty/URun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::Fall);
}

void URun::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URun::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URun::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URun::OnRunningReleased);
	Character->OnGrabbingStarted.AddDynamic(this,&URun::OnGrabbingStarted);
	Character->OnJumpingStarted.AddDynamic(this,&URun::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
}

void URun::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URun::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URun::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URun::OnRunningReleased);
	Character->OnGrabbingStarted.RemoveDynamic(this,&URun::OnGrabbingStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&URun::OnjumpingStarted);
}

void URun::OnMoving(FVector2D MoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * MoveInput.Y + Character->GetActorRightVector() * MoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void URun::OnRunningReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::Walk);
}

void URun::OnMovingReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::Idle);
}

void URun::OnGrabbingStarted()
{
	StateMachine->ChangeState(EState::RunGrab);
}

void URun::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::Jump);
}

