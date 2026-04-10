#include "StateMachine/States/UDeath.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UDeath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void UDeath::Enter()
{
	Super::Enter();
	Character->OnRevive.AddDynamic(this, &UDeath::OnRevive);
	
}

void UDeath::Exit()
{
	Super::Exit();
	Character->OnRevive.RemoveDynamic(this, &UDeath::OnRevive);
}

bool UDeath::CanUseGrab()
{
	return false;
}

bool UDeath::CanUseRecord()
{
	return false;
}

bool UDeath::CanUseInteract()
{
	return false;
}

void UDeath::OnRevive()
{
	StateMachine->ChangeState(EState::Revive);
}
