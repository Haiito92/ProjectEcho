#include "StateMachine/States/Throw/UWalkThrow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UWalkThrow::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UWalkThrow::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UWalkThrow::OnMovePressed);
	Character->OnMoveReleased.AddDynamic(this,&UWalkThrow::OnMoveReleased);
	Character->OnRunningStarted.AddDynamic(this,&UWalkThrow::OnRunningStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
	StateMachine->ChangeState(EState::Walk);
}

void UWalkThrow::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UWalkThrow::OnMovePressed);
	Character->OnMoveReleased.RemoveDynamic(this,&UWalkThrow::OnMoveReleased);
	Character->OnRunningStarted.RemoveDynamic(this,&UWalkThrow::OnRunningStarted);
}

void UWalkThrow::OnMoveReleased(bool IsReleased)
{
	StateMachine->ChangeState(EState::IdleThrow);
}

void UWalkThrow::OnRunningStarted(bool InRunningStarted)
{
	StateMachine->ChangeState(EState::RunThrow);
}

void UWalkThrow::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}
