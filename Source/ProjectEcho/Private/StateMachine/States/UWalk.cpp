#include "StateMachine/States/UWalk.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"

UWalk::UWalk()
{
	EnumState = EState::Walk;
}

void UWalk::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	CheckIsFalling();
}

void UWalk::Enter()
{
	Super::Enter();
	Character->OnMoveReleased.AddDynamic(this,&UWalk::OnMoveReleased);
	Character->OnMovePressed.AddDynamic(this,&UWalk::OnMovePressed);
	Character->OnRunningStarted.AddDynamic(this,&UWalk::OnRunningStarted);
	Character->OnJumpingStarted.AddDynamic(this,&UWalk::OnJumpingStarted);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void UWalk::Exit()
{
	Super::Exit();
	Character->OnMoveReleased.RemoveDynamic(this,&UWalk::OnMoveReleased);
	Character->OnMovePressed.RemoveDynamic(this,&UWalk::OnMovePressed);
	Character->OnRunningStarted.RemoveDynamic(this,&UWalk::OnRunningStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&UWalk::OnJumpingStarted);
}



void UWalk::OnRunningStarted(bool InRunningStarted)
{
	StateMachine->ChangeState(EState::Run);
}