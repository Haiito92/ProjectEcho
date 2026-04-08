#include "StateMachine/States/Grab/UIdleGrab.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleGrab::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UIdleGrab::Enter()
{
	Super::Enter();
	Character->OnMoveStarted.AddDynamic(this,&UIdleGrab::OnMoveStarted);
	StateMachine->ChangeState(EState::IdleHold);
}

void UIdleGrab::Exit()
{
	Super::Exit();
	Character->OnMoveStarted.RemoveDynamic(this,&UIdleGrab::OnMoveStarted);
}

void UIdleGrab::OnMoveStarted(bool IsPressed)
{
	StateMachine->ChangeState(EState::WalkGrab);
}

