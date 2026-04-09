#include "StateMachine/States/Throw/URunThrow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URunThrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallThrow);
}

void URunThrow::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URunThrow::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URunThrow::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URunThrow::OnRunningReleased);
	Character->OnJumpingStarted.AddDynamic(this,&URunThrow::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
	StateMachine->ChangeState(EState::Run);
}

void URunThrow::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URunThrow::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URunThrow::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URunThrow::OnRunningReleased);
	Character->OnJumpingStarted.RemoveDynamic(this,&URunThrow::OnjumpingStarted);
}

void URunThrow::OnMoving(FVector2D MoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * MoveInput.Y + Character->GetActorRightVector() * MoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void URunThrow::OnRunningReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::WalkThrow);
}

void URunThrow::OnMovingReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::IdleThrow);
}

void URunThrow::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpThrow);
}