#include "StateMachine/UStateMachine.h"
#include "StateMachine/States/UIdle.h"
#include "StateMachine/States/UMove.h"
#include "StateMachine/States/URun.h"
#include "StateMachine/States/UJump.h"
#include "StateMachine/States/UFall.h"
#include "StateMachine/States/UWallRun.h"

void UStateMachine::InitStates(ACharacterST* InCharacter)
{
	Idle = NewObject<UIdle>(this);
	Move = NewObject<UMove>(this);
	Run = NewObject<URun>(this);
	Jump = NewObject<UJump>(this);
	Fall = NewObject<UFall>(this);
	WallRun = NewObject<UWallRun>(this);
	
	Idle->InitStateMachine(this,InCharacter);
	Run->InitStateMachine(this,InCharacter);
	Jump->InitStateMachine(this,InCharacter);
	Fall->InitStateMachine(this,InCharacter);
	WallRun->InitStateMachine(this,InCharacter);
	Move->InitStateMachine(this,InCharacter);
	
	AddState(Idle,EState::Idle);
	AddState(Move,EState::Move);
	AddState(Run,EState::Sprint);
	AddState(Jump,EState::Jump);
	AddState(Fall,EState::Fall);
	AddState(WallRun,EState::WallRun);
	
	StartState(EState::Idle);
}

void UStateMachine::StartState(const EState& newState)
{
	if (UState** FoundState = StateMap.Find(newState))
	{
		CurrentState = *FoundState;
		CurrentState->Enter();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("State not found!"));
	}
}

void UStateMachine::AddState(UState* newState, const EState& nameState)
{
	if (!newState)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add NULL state"));
		return;
	}

	StateMap.Add(nameState, newState);
}

void UStateMachine::ChangeState(const EState& newState)
{
	if (CurrentState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Current State is null can't change state"));
		return;
	}
	
	CurrentState->Exit();
	PreviousState = CurrentState;
	CurrentState = StateMap[newState];
	CurrentState->Enter();
}

void UStateMachine::Tick(float DeltaTime)
{
	if (CurrentState == nullptr) 
		return;
	CurrentState->Tick(DeltaTime);
}


