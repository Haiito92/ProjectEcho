#include "StateMachine/States/UDeath.h"

#include "EchoSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"


void UDeath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void UDeath::Enter()
{
	Super::Enter();

	Character->DeathEnd();
}

void UDeath::Exit()
{
	Super::Exit();
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
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "Death to Revive", FColor::Blue, 3.0f);
	StateMachine->ChangeState(EState::Revive);
}
