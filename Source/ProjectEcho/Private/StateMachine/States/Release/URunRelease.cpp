#include "StateMachine/States/Release/URunRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URunRelease::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallRelease);
}

void URunRelease::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URunRelease::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URunRelease::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URunRelease::OnRunningReleased);
	Character->OnJumpingStarted.AddDynamic(this,&URunRelease::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
	StateMachine->ChangeState(EState::Run);
}

void URunRelease::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URunRelease::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URunRelease::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URunRelease::OnRunningReleased);
	Character->OnJumpingStarted.RemoveDynamic(this,&URunRelease::OnjumpingStarted);
}

void URunRelease::OnMoving(FVector2D MoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * MoveInput.Y + Character->GetActorRightVector() * MoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void URunRelease::OnRunningReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::WalkRelease);
}

void URunRelease::OnMovingReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::IdleRelease);
}

void URunRelease::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpRelease);
}