#include "StateMachine/States/Hold/UWalkHold.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

void UWalkHold::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::FallHold);
}

void UWalkHold::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UWalkHold::OnMovePressed);
	Character->OnMoveReleased.AddDynamic(this,&UWalkHold::OnMoveReleased);
	Character->OnRunningStarted.AddDynamic(this,&UWalkHold::OnRunningStarted);
	Character->OnThrowingStarted.AddDynamic(this,&UWalkHold::OnThrowingStarted);
	Character->OnGrabbingStarted.AddDynamic(this,&UWalkHold::OnReleaseStarted);
	Character->OnJumpingStarted.AddDynamic(this,&UWalkHold::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
}

void UWalkHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UWalkHold::OnMovePressed);
	Character->OnMoveReleased.RemoveDynamic(this,&UWalkHold::OnMoveReleased);
	Character->OnRunningStarted.RemoveDynamic(this,&UWalkHold::OnRunningStarted);
	Character->OnThrowingStarted.RemoveDynamic(this,&UWalkHold::OnThrowingStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UWalkHold::OnReleaseStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&UWalkHold::OnjumpingStarted);
}

void UWalkHold::OnMoveReleased(bool IsReleased)
{
	StateMachine->ChangeState(EState::IdleHold);
}

void UWalkHold::OnRunningStarted(bool InRunningStarted)
{
	StateMachine->ChangeState(EState::RunHold);
}

void UWalkHold::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UWalkHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::WalkThrow);
}

void UWalkHold::OnReleaseStarted()
{
	StateMachine->ChangeState(EState::WalkRelease);
}

void UWalkHold::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::JumpHold);
}