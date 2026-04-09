#include "StateMachine/States/Grab/URunGrab.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URunGrab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallGrab);
}

void URunGrab::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URunGrab::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URunGrab::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URunGrab::OnRunningReleased);
	Character->OnJumpingStarted.AddDynamic(this,&URunGrab::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
	StateMachine->ChangeState(EState::RunHold);
}

void URunGrab::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URunGrab::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URunGrab::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URunGrab::OnRunningReleased);
	Character->OnJumpingStarted.RemoveDynamic(this,&URunGrab::OnjumpingStarted);
}

void URunGrab::OnMoving(FVector2D MoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * MoveInput.Y + Character->GetActorRightVector() * MoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void URunGrab::OnRunningReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::WalkGrab);
}

void URunGrab::OnMovingReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::IdleGrab);
}

void URunGrab::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpGrab);
}

