#include "StateMachine/States/Release/UWalkRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UWalkRelease::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallRelease);
	
}

void UWalkRelease::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UWalkRelease::OnMovePressed);
	Character->OnMoveReleased.AddDynamic(this,&UWalkRelease::OnMoveReleased);
	Character->OnRunningStarted.AddDynamic(this,&UWalkRelease::OnRunningStarted);
	Character->OnJumpingStarted.AddDynamic(this,&UWalkRelease::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
	StateMachine->ChangeState(EState::Walk);
}

void UWalkRelease::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UWalkRelease::OnMovePressed);
	Character->OnMoveReleased.RemoveDynamic(this,&UWalkRelease::OnMoveReleased);
	Character->OnRunningStarted.RemoveDynamic(this,&UWalkRelease::OnRunningStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&UWalkRelease::OnjumpingStarted);
}

void UWalkRelease::OnMoveReleased(bool IsReleased)
{
	StateMachine->ChangeState(EState::IdleRelease);
}

void UWalkRelease::OnRunningStarted(bool InRunningStarted)
{
	StateMachine->ChangeState(EState::RunRelease);
}

void UWalkRelease::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UWalkRelease::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpRelease);
}
