#include "StateMachine/States/UFall.h"

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
	Character->OnMovePressed.AddDynamic(this,&UFall::OnMovePressed);
}

void UFall::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UFall::OnMovePressed);
}
