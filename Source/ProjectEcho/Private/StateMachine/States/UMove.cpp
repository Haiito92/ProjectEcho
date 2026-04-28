#include "StateMachine/States/UMove.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"


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

void UMove::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	Super::InitState(InStateMachine, InCharacter);
	EnumState = EState::Move;
	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
	else
	{
		
		UE_LOG(LogTemp, Warning, TEXT("No character attached!"));
	}
}
