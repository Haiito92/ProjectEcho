#include "StateMachine/States/UIdle.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdle::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UIdle::Enter()
{
	Super::Enter();
	Character->OnMoveStarted.AddDynamic(this,&UIdle::OnMoveStarted);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMoveStarted.RemoveDynamic(this,&UIdle::OnMoveStarted);
}

void UIdle::OnMoveStarted(bool IsPressed)
{
	StateMachine->ChangeState(EState::Walk);
}

