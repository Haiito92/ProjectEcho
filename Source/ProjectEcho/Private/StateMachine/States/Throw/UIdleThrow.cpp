#include "StateMachine/States/Throw/UIdleThrow.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleThrow::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallThrow);
}

void UIdleThrow::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdleThrow::OnMovePressed);
	Character->OnJumpingStarted.AddDynamic(this,&UIdleThrow::OnjumpingStarted);
	
	StateMachine->ChangeState(EState::Idle);
}

void UIdleThrow::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleThrow::OnMovePressed);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdleThrow::OnjumpingStarted);
}

void UIdleThrow::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkThrow);
}

void UIdleThrow::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpThrow);
}