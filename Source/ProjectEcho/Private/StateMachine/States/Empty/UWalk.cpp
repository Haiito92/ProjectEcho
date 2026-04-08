#include "StateMachine/States/Empty/UWalk.h"
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
	Character->OnMovePressed.AddDynamic(this,&UWalk::OnMovePressed);
	Character->OnMoveReleased.AddDynamic(this,&UWalk::OnMoveReleased);
	Character->OnRunningStarted.AddDynamic(this,&UWalk::OnRunningStarted);
	Character->OnGrabbingStarted.AddDynamic(this,&UWalk::OnGrabbingStarted);
	Character->OnJumpingStarted.AddDynamic(this,&UWalk::OnjumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->WalkSpeed;
}

void UWalk::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UWalk::OnMovePressed);
	Character->OnMoveReleased.RemoveDynamic(this,&UWalk::OnMoveReleased);
	Character->OnRunningStarted.RemoveDynamic(this,&UWalk::OnRunningStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UWalk::OnGrabbingStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&UWalk::OnjumpingStarted);
}

void UWalk::OnMoveReleased(bool IsReleased)
{
	StateMachine->ChangeState(EState::Idle);
}

void UWalk::OnRunningStarted(bool InRunningStarted)
{
	StateMachine->ChangeState(EState::Run);
}

void UWalk::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UWalk::OnGrabbingStarted()
{
	StateMachine->ChangeState(EState::WalkGrab);
}

void UWalk::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::Jump);
}
