#include "Public/StateMachine/UStateMachine.h"

#include "GameFramework/Character.h"


void UStateMachine::BeginPlay()
{
	InitStates();
}

UStateMachine::UStateMachine()
{
	Owner = Cast<ACharacter>(GetOwner());
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UStateMachine::TickComponent(float DeltaTime, enum ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentState == nullptr) return;
	
	CurrentState->Tick(DeltaTime);
}

void UStateMachine::InitStates()
{
}

void UStateMachine::StartState(const FString& newState)
{
	if (UState** FoundState = StateMap.Find(newState))
	{
		CurrentState = *FoundState;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("State '%s' not found!"), *newState);
	}
}

void UStateMachine::AddState(UState* newState,const FString& nameState)
{
	if (!newState)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add NULL state: %s"), *nameState);
		return;
	}

	StateMap.Add(nameState, newState);
}


void UStateMachine::ChangeState(const FString& newState)
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


