#include "StateMachine/States/UIdle.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdle::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	CheckIsFalling();
}

void UIdle::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdle::OnMovePressed);
	Character->OnJumpingStarted.AddDynamic(this,&UIdle::OnJumpingStarted);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdle::OnMovePressed);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdle::OnJumpingStarted);
}

void UIdle::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::Walk);
}

