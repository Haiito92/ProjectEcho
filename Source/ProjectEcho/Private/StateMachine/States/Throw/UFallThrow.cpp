#include "StateMachine/States/Throw/UFallThrow.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UFallThrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
		StateMachine->ChangeState(EState::IdleThrow);
}


void UFallThrow::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UFallThrow::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UFallThrow::OnMovePressed);
}

void UFallThrow::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UFallThrow::OnMovePressed);
}
