#include "Public/StateMachine/StateMachine.h"

void StateMachine::InitStates()
{
	
}

void StateMachine::StartState(const std::string& newState)
{
	PtrCurrentState = StateMap[newState];
}

void StateMachine::AddState(State* newState,const std::string& stateName)
{
	StateMap[stateName] = newState;
}

void StateMachine::ChangeState(const std::string& newState)
{
	if (PtrCurrentState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("StateMachine::ChangeState - No current state"));
		return;
	}
	PtrCurrentState->Exit();
	PtrPreviousState = PtrCurrentState;
	PtrCurrentState = StateMap[newState];
	PtrCurrentState->Enter();
}

void StateMachine::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PtrCurrentState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("StateMachine::ChangeState - No current state"));
		return;
	}
	else
	{
		PtrCurrentState->Tick(DeltaTime);
	}
}
