#include "StateMachine/States/Empty/UFall.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UFall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
		StateMachine->ChangeState(EState::Idle);
}

void UFall::Enter()
{
	Super::Enter();
}

void UFall::Exit()
{
	Super::Exit();
}
