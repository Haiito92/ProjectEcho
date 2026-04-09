#include "StateMachine/States/Hold/UIdleHold.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleHold::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallHold);
}

void UIdleHold::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdleHold::OnMovePressed);
	Character->OnThrowingStarted.AddDynamic(this,&UIdleHold::OnThrowingStarted);
	Character->OnGrabbingStarted.AddDynamic(this,&UIdleHold::OnReleaseStarted);
	Character->OnJumpingStarted.AddDynamic(this,&UIdleHold::OnjumpingStarted);
}

void UIdleHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleHold::OnMovePressed);
	Character->OnThrowingStarted.RemoveDynamic(this,&UIdleHold::OnThrowingStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UIdleHold::OnReleaseStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdleHold::OnjumpingStarted);
}

void UIdleHold::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkHold);
}

void UIdleHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::IdleThrow);
}

void UIdleHold::OnReleaseStarted()
{
	StateMachine->ChangeState(EState::IdleRelease);
}

void UIdleHold::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpHold);
}
