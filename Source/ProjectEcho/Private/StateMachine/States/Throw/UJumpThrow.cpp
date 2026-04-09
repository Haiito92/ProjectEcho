#include "StateMachine/States/Throw/UJumpThrow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UJumpThrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallThrow);
}

void UJumpThrow::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UJumpThrow::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UJumpThrow::OnMovePressed);
	Character->Jump();
}

void UJumpThrow::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UJumpThrow::OnMovePressed);
}
