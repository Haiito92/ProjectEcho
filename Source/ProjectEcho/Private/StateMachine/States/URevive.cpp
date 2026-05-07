#include "StateMachine/States/URevive.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"


void URevive::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	Super::InitState(InStateMachine, InCharacter);
	EnumState = EState::Revive;
	StateSettings = EStateSettings::None;
}

void URevive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void URevive::Enter()
{
	StateMachine->ChangeState(EState::Idle);
	Character->Life = 100;
}

void URevive::Exit()
{
}