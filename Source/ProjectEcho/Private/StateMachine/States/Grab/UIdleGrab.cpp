#include "StateMachine/States/Grab/UIdleGrab.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleGrab::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallGrab);
}

void UIdleGrab::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdleGrab::OnMovePressed);
	Character->OnJumpingStarted.AddDynamic(this,&UIdleGrab::OnjumpingStarted);
	StateMachine->ChangeState(EState::IdleHold);
}

void UIdleGrab::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleGrab::OnMovePressed);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdleGrab::OnjumpingStarted);
}

void UIdleGrab::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkGrab);
}

void UIdleGrab::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpGrab);
}

