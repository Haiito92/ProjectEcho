#include "StateMachine/States/UWalk.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UWalk::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UWalk::Enter()
{
	Super::Enter();
	Character->OnMoveInput.AddDynamic(this,&UWalk::OnMove);
	Character->OnMoveReleased.AddDynamic(this,&UWalk::OnMoveReleased);
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
}

void UWalk::Exit()
{
	Super::Exit();
	Character->OnMoveInput.RemoveDynamic(this,&UWalk::OnMove);
	Character->OnMoveReleased.RemoveDynamic(this,&UWalk::OnMoveReleased);
}

void UWalk::OnMoveReleased(bool IsReleased)
{
	StateMachine->ChangeState(EState::Idle);
}

void UWalk::OnMove(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}
