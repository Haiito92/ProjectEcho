#include "StateMachine/UStateMachine.h"

#include "EchoSystem.h"
#include "StateMachine/States/Empty/UIdle.h"
#include "StateMachine/States/Empty/UWalk.h"
#include "StateMachine/States/Empty/URun.h"
#include "StateMachine/States/Empty/UJump.h"
#include "StateMachine/States/Empty/UFall.h"
#include "StateMachine/States/Empty/UWallRun.h"

#include "StateMachine/States/Throw/UIdleThrow.h"
#include "StateMachine/States/Throw/UWalkThrow.h"
#include "StateMachine/States/Throw/URunThrow.h"
#include "StateMachine/States/Throw/UJumpThrow.h"
#include "StateMachine/States/Throw/UFallThrow.h"

#include "StateMachine/States/Grab/UIdleGrab.h"
#include "StateMachine/States/Grab/UWalkGrab.h"
#include "StateMachine/States/Grab/URunGrab.h"
#include "StateMachine/States/Grab/UJumpGrab.h"
#include "StateMachine/States/Grab/UFallGrab.h"

#include "StateMachine/States/Hold/UIdleHold.h"
#include "StateMachine/States/Hold/UWalkHold.h"
#include "StateMachine/States/Hold/URunHold.h"
#include "StateMachine/States/Hold/UJumpHold.h"
#include "StateMachine/States/Hold/UFallHold.h"

#include "StateMachine/States/Release/UIdleRelease.h"
#include "StateMachine/States/Release/URunRelease.h"
#include "StateMachine/States/Release/UWalkRelease.h"
#include "StateMachine/States/Release/UJumpRelease.h"
#include "StateMachine/States/Release/UFallRelease.h"

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
	
	IdleGrab = NewObject<UIdleGrab>(this);
	WalkGrab = NewObject<UWalkGrab>(this);
	RunGrab = NewObject<URunGrab>(this);
	JumpGrab = NewObject<UJumpGrab>(this);
	FallGrab = NewObject<UFallGrab>(this);
	
	IdleHold = NewObject<UIdleHold>(this);
	WalkHold = NewObject<UWalkHold>(this);
	RunHold = NewObject<URunHold>(this);
	JumpHold = NewObject<UJumpHold>(this);
	FallHold = NewObject<UFallHold>(this);
	
	IdleThrow = NewObject<UIdleThrow>(this);
	WalkThrow = NewObject<UWalkThrow>(this);
	RunThrow = NewObject<URunThrow>(this);
	JumpThrow = NewObject<UJumpThrow>(this);
	FallThrow = NewObject<UFallThrow>(this);
	
	IdleRelease = NewObject<UIdleRelease>(this);
	WalkRelease = NewObject<UWalkRelease>(this);
	RunRelease = NewObject<URunRelease>(this);
	JumpRelease = NewObject<UJumpRelease>(this);
	FallRelease = NewObject<UFallRelease>(this);
	
	Idle->InitState(this,InCharacter);
	Walk->InitState(this,InCharacter);
	Run->InitState(this,InCharacter);
	Jump->InitState(this,InCharacter);
	Fall->InitState(this,InCharacter);
	WallRun->InitState(this,InCharacter);
	
	
	IdleGrab->InitState(this,InCharacter);
	WalkGrab->InitState(this,InCharacter);
	RunGrab->InitState(this,InCharacter);
	JumpGrab->InitState(this,InCharacter);
	FallGrab->InitState(this,InCharacter);
	
	IdleThrow->InitState(this,InCharacter);
	WalkThrow->InitState(this,InCharacter);
	RunThrow->InitState(this,InCharacter);
	JumpThrow->InitState(this,InCharacter);
	FallThrow->InitState(this,InCharacter);
	
	IdleHold->InitState(this,InCharacter);
	WalkHold->InitState(this,InCharacter);
	RunHold->InitState(this,InCharacter);
	JumpHold->InitState(this,InCharacter);
	FallHold->InitState(this,InCharacter);
	
	IdleRelease->InitState(this,InCharacter);
	WalkRelease->InitState(this,InCharacter);
	RunRelease->InitState(this,InCharacter);
	JumpRelease->InitState(this,InCharacter);
	FallRelease->InitState(this,InCharacter);
	
	AddState(Idle,EState::Idle);
	AddState(Walk,EState::Walk);
	AddState(Run,EState::Run);
	AddState(Jump,EState::Jump);
	AddState(Fall,EState::Fall);
	AddState(WallRun,EState::WallRun);
	
	AddState(IdleGrab,EState::IdleGrab);
	AddState(WalkGrab,EState::WalkGrab);
	AddState(RunGrab,EState::RunGrab);
	AddState(JumpGrab,EState::JumpGrab);
	AddState(FallGrab,EState::FallGrab);
	
	AddState(IdleHold,EState::IdleHold);
	AddState(WalkHold,EState::WalkHold);
	AddState(RunHold,EState::RunHold);
	AddState(JumpHold,EState::JumpHold);
	AddState(FallHold,EState::FallHold);
	
	AddState(IdleThrow,EState::IdleThrow);
	AddState(WalkThrow,EState::WalkThrow);
	AddState(RunThrow,EState::RunThrow);
	AddState(JumpThrow,EState::JumpThrow);
	AddState(FallThrow,EState::FallThrow);
	
	AddState(IdleRelease,EState::IdleRelease);
	AddState(WalkRelease,EState::WalkRelease);
	AddState(RunRelease,EState::RunRelease);
	AddState(JumpRelease,EState::JumpRelease);
	AddState(FallRelease,EState::FallRelease);
	
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


