#include "StateMachine/States/Release/UFallRelease.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UFallRelease::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
		StateMachine->ChangeState(EState::IdleRelease);
}


void UFallRelease::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UFallRelease::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UFallRelease::OnMovePressed);
}

void UFallRelease::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UFallRelease::OnMovePressed);
}
