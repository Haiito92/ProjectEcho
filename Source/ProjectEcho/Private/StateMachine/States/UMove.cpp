#include "StateMachine/States/UMove.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UMove::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UMove::Enter()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Idle"));
	Super::Enter();
	Character->OnMoveInput.AddDynamic(this,&UMove::OnMove);
}

void UMove::Exit()
{
	Super::Exit();
	Character->OnMoveInput.RemoveDynamic(this,&UMove::OnMove);
}

void UMove::OnMove(FVector2D InMoveInput)
{
	if (InMoveInput == FVector2D::ZeroVector)
	{
		StateMachine->ChangeState(EState::Idle);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Move"));
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}
