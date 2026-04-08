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
	Character->OnMovePressed.AddDynamic(this,&UIdleHold::OnMovePressed);
	Character->OnThrowingStarted.AddDynamic(this,&UIdleHold::OnThrowingStarted);
}

void UIdleHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdleHold::OnMovePressed);
	Character->OnThrowingStarted.RemoveDynamic(this,&UIdleHold::OnThrowingStarted);
}

void UIdleHold::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::WalkHold);
}

void UIdleHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::IdleThrow);
}

