#include "StateMachine/States/Hold/UIdleHold.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleHold::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UIdleHold::Enter()
{
	Super::Enter();
	Character->OnMoveStarted.AddDynamic(this,&UIdleHold::OnMoveStarted);
	Character->OnThrowingStarted.AddDynamic(this,&UIdleHold::OnThrowingStarted);
}

void UIdleHold::Exit()
{
	Super::Exit();
	Character->OnMoveStarted.RemoveDynamic(this,&UIdleHold::OnMoveStarted);
	Character->OnThrowingStarted.RemoveDynamic(this,&UIdleHold::OnThrowingStarted);
}

void UIdleHold::OnMoveStarted(bool IsPressed)
{
	StateMachine->ChangeState(EState::WalkHold);
}

void UIdleHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::IdleThrow);
}

