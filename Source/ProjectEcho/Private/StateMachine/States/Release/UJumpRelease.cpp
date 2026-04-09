#include "StateMachine/States/Release/UJumpRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UJumpRelease::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallRelease);
}

void UJumpRelease::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UJumpRelease::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UJumpRelease::OnMovePressed);
	Character->Jump();
}

void UJumpRelease::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UJumpRelease::OnMovePressed);
}
