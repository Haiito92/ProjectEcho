#include "StateMachine/States/Empty/UIdle.h"
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
	Character->OnGrabbingStarted.AddDynamic(this,&UIdle::OnGrabbingStarted);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMoveStarted.RemoveDynamic(this,&UIdle::OnMoveStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UIdle::OnGrabbingStarted);
}

void UIdle::OnMoveStarted(bool IsPressed)
{
	StateMachine->ChangeState(EState::Walk);
}

void UIdle::OnGrabbingStarted()
{
	StateMachine->ChangeState(EState::IdleGrab);
}

