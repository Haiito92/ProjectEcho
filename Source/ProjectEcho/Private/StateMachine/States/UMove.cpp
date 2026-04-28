#include "StateMachine/States/UMove.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

UMove::UMove()
{
	EnumState = EState::Move;
	if (Character != nullptr)
		Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void UMove::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this, &UMove::OnMovePressed);
	Character->OnMoveReleased.AddDynamic(this, &UMove::OnMoveReleased);
	Character->OnJumpingStarted.AddDynamic(this, &UMove::OnJumpingStarted);
}

void UMove::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this, &UMove::OnMovePressed);
	Character->OnMoveReleased.RemoveDynamic(this, &UMove::OnMoveReleased);
	Character->OnJumpingStarted.RemoveDynamic(this, &UMove::OnJumpingStarted);
}
