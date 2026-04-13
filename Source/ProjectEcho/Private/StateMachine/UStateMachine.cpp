#include "StateMachine/UStateMachine.h"
#include "EchoSystem.h"
#include "StateMachine/States/UDeath.h"

#include "StateMachine/States/UIdle.h"
#include "StateMachine/States/UWalk.h"
#include "StateMachine/States/URun.h"
#include "StateMachine/States/UJump.h"
#include "StateMachine/States/UFall.h"
#include "StateMachine/States/URevive.h"
#include "StateMachine/States/URewind.h"
#include "StateMachine/States/UWallRun.h"

#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UStateMachine::InitStates(ACharacterST* InCharacter)
{
	Idle = NewObject<UIdle>(this);
	Walk = NewObject<UWalk>(this);
	Run = NewObject<URun>(this);
	Jump = NewObject<UJump>(this);
	Fall = NewObject<UFall>(this);
	WallRun = NewObject<UWallRun>(this);
	Death = NewObject<UDeath>(this);
	Revive = NewObject<URevive>(this);
	Rewind = NewObject<URewind>(this);
	
	Idle->InitState(this,InCharacter);
	Walk->InitState(this,InCharacter);
	Run->InitState(this,InCharacter);
	Jump->InitState(this,InCharacter);
	Fall->InitState(this,InCharacter);
	WallRun->InitState(this,InCharacter);
	Death->InitState(this,InCharacter);
	Revive->InitState(this,InCharacter);
	Rewind->InitState(this,InCharacter);
	
	AddState(Idle,EState::Idle);
	AddState(Walk,EState::Walk);
	AddState(Run,EState::Run);
	AddState(Jump,EState::Jump);
	AddState(Fall,EState::Fall);
	AddState(WallRun,EState::WallRun);
	AddState(Death,EState::Death);
	AddState(Revive,EState::Revive);
	AddState(Rewind,EState::Rewind);
	
	
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
	
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine,EEchoMessageType::Log,UEnum::GetValueAsString(newState), FColor::White, 3.0f);
	
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


