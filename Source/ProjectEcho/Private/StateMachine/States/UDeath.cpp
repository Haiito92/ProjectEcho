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
	Character->OnRevive.AddDynamic(this, &UDeath::OnRevive);
}

void UDeath::Exit()
{
	Character->OnRevive.RemoveDynamic(this, &UDeath::OnRevive);
}

void UDeath::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	Super::InitState(InStateMachine, InCharacter);
	EnumState = EState::Death;
	StateSettings = EStateSettings::None;
}

void UDeath::OnRevive()
{
	Character->DeathEnd();
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine, EEchoMessageType::Log, "Death to Revive", FColor::Blue, 3.0f);
	StateMachine->ChangeState(EState::Revive);
}
