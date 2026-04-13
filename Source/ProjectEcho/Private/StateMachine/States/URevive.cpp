#include "StateMachine/States/URevive.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void URevive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void URevive::Enter()
{
	Super::Enter();	
	StateMachine->ChangeState(EState::Idle);
	Character->Life = 100;
}

void URevive::Exit()
{
	Super::Exit();
}

bool URevive::CanUseGrab()
{
	return false;
}

bool URevive::CanUseRecord()
{
	return false;
}

bool URevive::CanUseInteract()
{
	return false;
}
