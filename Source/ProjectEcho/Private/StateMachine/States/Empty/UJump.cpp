#include "StateMachine/States/Empty/UJump.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::Fall);
}

void UJump::Enter()
{
	Super::Enter();
	Character->Jump();
}

void UJump::Exit()
{
	Super::Exit();
}
