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
	Character->OnMovePressed.AddDynamic(this,&UIdleThrow::OnMovePressed);
	StateMachine->ChangeState(EState::Idle);
}

void UIdleThrow::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleThrow::OnMovePressed);
}

void UIdleThrow::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkThrow);
}

