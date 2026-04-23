#include "StateMachine/States/URevive.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

URevive::URevive()
{
	EnumState = EState::Revive;
	StateSettings = EStateSettings::None;
}

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
	Character->ActivateCharacterInput();
}