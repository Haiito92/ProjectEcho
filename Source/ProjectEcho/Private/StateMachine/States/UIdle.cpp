#include "StateMachine/States/UIdle.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdle::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (MoveInput == FVector2D::ZeroVector) return;
	StateMachine->ChangeState(EState::Walk);
}

void UIdle::Enter()
{
	Super::Enter();
	Character->OnMoveInput.AddDynamic(this,&UIdle::OnMove);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMoveInput.RemoveDynamic(this,&UIdle::OnMove);
}

void UIdle::OnMove(FVector2D InMoveInput)
{
	MoveInput = InMoveInput;
}

