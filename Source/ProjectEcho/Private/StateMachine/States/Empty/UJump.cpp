#include "StateMachine/States/Empty/UJump.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::Fall);
}

void UJump::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UJump::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UJump::OnMovePressed);
	Character->Jump();
}

void UJump::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UJump::OnMovePressed);
}
