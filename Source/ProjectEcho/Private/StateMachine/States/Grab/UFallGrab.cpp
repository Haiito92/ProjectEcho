#include "StateMachine/States/Grab/UFallGrab.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UFallGrab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
		StateMachine->ChangeState(EState::IdleGrab);
}


void UFallGrab::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UFallGrab::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UFallGrab::OnMovePressed);
}

void UFallGrab::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UFallGrab::OnMovePressed);
}
