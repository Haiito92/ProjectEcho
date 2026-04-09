#include "StateMachine/States/Empty/UIdle.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdle::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(EState::Fall);
}

void UIdle::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdle::OnMovePressed);
	Character->OnGrabbingStarted.AddDynamic(this,&UIdle::OnGrabbingStarted);
	Character->OnJumpingStarted.AddDynamic(this,&UIdle::OnjumpingStarted);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdle::OnMovePressed);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UIdle::OnGrabbingStarted);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdle::OnjumpingStarted);
}

void UIdle::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::Walk);
}

void UIdle::OnGrabbingStarted()
{
	StateMachine->ChangeState(EState::IdleGrab);
}

void UIdle::OnjumpingStarted()
{
	StateMachine->ChangeState(EState::Jump);
}

