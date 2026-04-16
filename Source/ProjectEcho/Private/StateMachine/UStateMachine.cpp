#include "StateMachine/UStateMachine.h"

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "StateMachine/Data/UStateMachineSettings.h"
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
	if (const UDataAssetDeveloperSettings* DataAssetSettings = GetDefault<UDataAssetDeveloperSettings>())
	{
		CurrentStateMachineSettings = DataAssetSettings->StateMachineData.LoadSynchronous();
	}
	TArray<EState> states;
	
	for (TSubclassOf<UState> s : CurrentStateMachineSettings->InstantiateStates)
	{
		UState* state = NewObject<UState>(this, s);
		if (states.Contains(state->EnumState))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine,EEchoMessageType::Error,"A State exist in time");
			continue;
		}
		
		states.Add(state->EnumState);
		state->InitState(this, InCharacter);
		StateMap[state->EnumState] = state;
	}
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
	if (StateMap.Find(newState) == nullptr) return;
	
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
	
	OnChangeState.Broadcast(newState);
}

void UStateMachine::Tick(float DeltaTime)
{
	if (CurrentState == nullptr) 
		return;
	CurrentState->Tick(DeltaTime);
}


