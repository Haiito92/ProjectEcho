#include "StateMachine/States/Grab/UJumpGrab.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UJumpGrab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallGrab);
}

void UJumpGrab::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UJumpGrab::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UJumpGrab::OnMovePressed);
	Character->Jump();
}

void UJumpGrab::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UJumpGrab::OnMovePressed);
}
