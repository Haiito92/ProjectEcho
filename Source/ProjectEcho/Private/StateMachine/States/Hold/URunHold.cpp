#include "StateMachine/States/Hold/URunHold.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URunHold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void URunHold::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URunHold::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URunHold::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URunHold::OnRunningReleased);
	Character->OnThrowingStarted.AddDynamic(this,&URunHold::OnThrowingStarted);
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
}

void URunHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URunHold::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URunHold::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URunHold::OnRunningReleased);
	Character->OnThrowingStarted.RemoveDynamic(this,&URunHold::OnThrowingStarted);
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

