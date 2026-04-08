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
	Character->OnMovePressed.AddDynamic(this,&UIdleGrab::OnMovePressed);
	StateMachine->ChangeState(EState::IdleHold);
}

void UIdleGrab::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleGrab::OnMovePressed);
}

void UIdleGrab::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkGrab);
}

