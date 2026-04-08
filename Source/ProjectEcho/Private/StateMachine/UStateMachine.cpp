#include "StateMachine/UStateMachine.h"
#include "StateMachine/States/Empty/UIdle.h"
#include "StateMachine/States/Empty/UWalk.h"
#include "StateMachine/States/Empty/URun.h"
#include "StateMachine/States/Empty/UJump.h"
#include "StateMachine/States/Empty/UFall.h"
#include "StateMachine/States/Empty/UWallRun.h"

#include "StateMachine/States/Throw/UIdleThrow.h"
#include "StateMachine/States/Throw/UWalkThrow.h"
#include "StateMachine/States/Throw/URunThrow.h"

#include "StateMachine/States/Grab/UIdleGrab.h"
#include "StateMachine/States/Grab/UWalkGrab.h"
#include "StateMachine/States/Grab/URunGrab.h"

#include "StateMachine/States/Hold/UIdleHold.h"
#include "StateMachine/States/Hold/UWalkHold.h"
#include "StateMachine/States/Hold/URunHold.h"

void UStateMachine::InitStates(ACharacterST* InCharacter)
{
	Idle = NewObject<UIdle>(this);
	Walk = NewObject<UWalk>(this);
	Run = NewObject<URun>(this);
	Jump = NewObject<UJump>(this);
	Fall = NewObject<UFall>(this);
	WallRun = NewObject<UWallRun>(this);
	
	IdleGrab = NewObject<UIdleGrab>(this);
	WalkGrab = NewObject<UWalkGrab>(this);
	RunGrab = NewObject<URunGrab>(this);
	
	IdleHold = NewObject<UIdleHold>(this);
	WalkHold = NewObject<UWalkHold>(this);
	RunHold = NewObject<URunHold>(this);
	
	IdleThrow = NewObject<UIdleThrow>(this);
	WalkThrow = NewObject<UWalkThrow>(this);
	RunThrow = NewObject<URunThrow>(this);
	
	Idle->InitState(this,InCharacter);
	Walk->InitState(this,InCharacter);
	Run->InitState(this,InCharacter);
	Jump->InitState(this,InCharacter);
	Fall->InitState(this,InCharacter);
	WallRun->InitState(this,InCharacter);
	
	
	IdleGrab->InitState(this,InCharacter);
	WalkGrab->InitState(this,InCharacter);
	RunGrab->InitState(this,InCharacter);
	
	IdleThrow->InitState(this,InCharacter);
	WalkThrow->InitState(this,InCharacter);
	RunThrow->InitState(this,InCharacter);
	
	IdleHold->InitState(this,InCharacter);
	WalkHold->InitState(this,InCharacter);
	RunHold->InitState(this,InCharacter);
	
	AddState(Idle,EState::Idle);
	AddState(Walk,EState::Walk);
	AddState(Run,EState::Run);
	AddState(Jump,EState::Jump);
	AddState(Fall,EState::Fall);
	AddState(WallRun,EState::WallRun);
	
	AddState(IdleGrab,EState::IdleGrab);
	AddState(WalkGrab,EState::WalkGrab);
	AddState(RunGrab,EState::RunGrab);
	
	AddState(IdleHold,EState::IdleHold);
	AddState(WalkHold,EState::WalkHold);
	AddState(RunHold,EState::RunHold);
	
	AddState(IdleThrow,EState::IdleThrow);
	AddState(WalkThrow,EState::WalkThrow);
	AddState(RunThrow,EState::RunThrow);
	
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
	newState->EnumState = nameState;
	StateMap.Add(nameState, newState);
}

void UStateMachine::ChangeState(const EState& newState)
{
	if (CurrentState->EnumState == newState) return;
	
	//TODO : Check state exist
	
	if (CurrentState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Current State is null can't change state"));
		return;
	}
	
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, UEnum::GetValueAsString(newState));
	
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


