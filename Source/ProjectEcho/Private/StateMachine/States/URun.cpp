#include "StateMachine/States/URun.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckIsFalling();
}

void URun::Enter()
{
	Super::Enter();
	Character->OnMoveReleased.AddDynamic(this,&URun::OnMoveReleased);
	Character->OnMovePressed.AddDynamic(this,&URun::OnMovePressed);
	Character->OnRunningReleased.AddDynamic(this,&URun::OnRunningReleased);
	Character->OnJumpingStarted.AddDynamic(this,&URun::OnJumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void URun::Exit()
{
	Super::Exit();
	Character->OnMoveReleased.RemoveDynamic(this,&URun::OnMoveReleased);
	Character->OnMovePressed.RemoveDynamic(this,&URun::OnMovePressed);
	Character->OnRunningReleased.RemoveDynamic(this,&URun::OnRunningReleased);
	Character->OnJumpingStarted.RemoveDynamic(this,&URun::OnJumpingStarted);
}

void URun::OnRunningReleased(bool InRunning)
{
	StateMachine->ChangeState(EState::Walk);
}




