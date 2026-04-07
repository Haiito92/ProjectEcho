#include "StateMachine/States/URun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void URun::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&URun::OnMoving);
	Character->OnMoveReleased.AddDynamic(this,&URun::OnMovingReleased);
	Character->OnRunningReleased.AddDynamic(this,&URun::OnRunningReleased);
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
}

void URun::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&URun::OnMoving);
	Character->OnMoveReleased.RemoveDynamic(this,&URun::OnMovingReleased);
	Character->OnRunningReleased.RemoveDynamic(this,&URun::OnRunningReleased);
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

