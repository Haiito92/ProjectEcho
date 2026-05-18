#include "StateMachine/States/URewind.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/ACharacterST.h"


void URewind::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	Super::InitState(InStateMachine, InCharacter);
	EnumState = EState::Rewind;
	StateSettings ^= EStateSettings::CanBeReflected;
	StateSettings ^= EStateSettings::CanBePropulsed;
}

void URewind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void URewind::Enter()
{
	Character->GetCharacterMovement()->DisableMovement();
	Character->GetCharacterMovement()->StopMovementImmediately();
	RecordManagerSubsystem->OnStopPlayerRewinding.AddDynamic(this, &URewind::OnRewindingEnded);
}

void URewind::Exit()
{
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	RecordManagerSubsystem->OnStopPlayerRewinding.RemoveDynamic(this, &URewind::OnRewindingEnded);
}

void URewind::OnRewindingEnded()
{
	StateMachine->ChangeState(EState::Idle);
}
