#include "StateMachine/States/Hub/UWalkHUB.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UWalkHUB::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UWalkHUB::Enter()
{
	Super::Enter();
}

void UWalkHUB::Exit()
{
	Super::Exit();
}