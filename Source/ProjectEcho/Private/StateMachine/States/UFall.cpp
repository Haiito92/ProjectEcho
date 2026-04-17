#include "StateMachine/States/UFall.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

UFall::UFall()
{
	EnumState = EState::Fall;
}

void UFall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		if (Character->MoveInputDir != FVector2D::ZeroVector)
		{
			if (Character->IsRunInputOn)
				StateMachine->ChangeState(EState::Run);
			else
				StateMachine->ChangeState(EState::Walk);
		}
		else
			StateMachine->ChangeState(EState::Idle);
	}
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