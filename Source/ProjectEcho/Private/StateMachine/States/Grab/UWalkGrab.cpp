#include "StateMachine/States/Grab/UWalkGrab.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UWalkGrab::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	
}

void UWalkGrab::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UWalkGrab::OnMovePressed);
	Character->OnMoveReleased.AddDynamic(this,&UWalkGrab::OnMoveReleased);
	Character->OnRunningStarted.AddDynamic(this,&UWalkGrab::OnRunningStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
	StateMachine->ChangeState(EState::WalkHold);
}

void UWalkGrab::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UWalkGrab::OnMovePressed);
	Character->OnMoveReleased.RemoveDynamic(this,&UWalkGrab::OnMoveReleased);
	Character->OnRunningStarted.RemoveDynamic(this,&UWalkGrab::OnRunningStarted);
}

void UWalkGrab::OnMoveReleased(bool IsReleased)
{
	StateMachine->ChangeState(EState::IdleGrab);
}

void UWalkGrab::OnRunningStarted(bool InRunningStarted)
{
	StateMachine->ChangeState(EState::RunGrab);
}

void UWalkGrab::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}
