#include "StateMachine/States/Empty/UFall.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UFall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
		StateMachine->ChangeState(EState::Idle);
}


void UFall::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
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
