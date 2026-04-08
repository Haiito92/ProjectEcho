#include "StateMachine/States/Hub/UIdleHUB.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdleHUB::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UIdleHUB::Enter()
{
	Super::Enter();
}

void UIdleHUB::Exit()
{
	Super::Exit();
}

