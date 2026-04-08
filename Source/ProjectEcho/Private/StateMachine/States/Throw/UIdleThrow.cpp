#include "StateMachine/States/Throw/UIdleThrow.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleThrow::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UIdleThrow::Enter()
{
	Super::Enter();
	Character->OnMoveStarted.AddDynamic(this,&UIdleThrow::OnMoveStarted);
	StateMachine->ChangeState(EState::Idle);
}

void UIdleThrow::Exit()
{
	Super::Exit();
	Character->OnMoveStarted.RemoveDynamic(this,&UIdleThrow::OnMoveStarted);
}

void UIdleThrow::OnMoveStarted(bool IsPressed)
{
	StateMachine->ChangeState(EState::WalkThrow);
}

