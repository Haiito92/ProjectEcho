#include "Public/StateMachine/UState.h"
#include "StateMachine/UPlayerStateMachine.h"

class UPlayerStateMachine;

UState::UState()
{
}

void UState::InitStateMachine(UPlayerStateMachine* sm)
{
	StateMachine = sm;
}

void UState::Enter()
{
}

void UState::Tick(float DeltaTime)
{
}

void UState::Exit()
{
}
