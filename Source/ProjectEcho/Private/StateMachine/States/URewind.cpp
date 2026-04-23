#include "StateMachine/States/URewind.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

URewind::URewind()
{
	EnumState = EState::Rewind;
	StateSettings ^= EStateSettings::CanBeReflected;
}


void URewind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void URewind::Enter()
{
	Super::Enter();
	Character->GetCharacterMovement()->DisableMovement();
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->DeactivateCharacterInput();
}

void URewind::Exit()
{
	Super::Exit();
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Character->ActivateCharacterInput();
}