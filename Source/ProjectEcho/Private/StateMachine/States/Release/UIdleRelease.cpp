#include "StateMachine/States/Release/UIdleRelease.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleRelease::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallRelease);
}

void UIdleRelease::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdleRelease::OnMovePressed);
	Character->OnJumpingStarted.AddDynamic(this,&UIdleRelease::OnjumpingStarted);
	StateMachine->ChangeState(EState::Idle);
}

void UIdleRelease::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleRelease::OnMovePressed);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdleRelease::OnjumpingStarted);
}

void UIdleRelease::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkRelease);
}

void UIdleRelease::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpRelease);
}