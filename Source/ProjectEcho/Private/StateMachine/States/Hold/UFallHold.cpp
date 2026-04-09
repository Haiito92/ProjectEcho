#include "StateMachine/States/Hold/UFallHold.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"

void UFallHold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround())
		StateMachine->ChangeState(EState::IdleHold);
}


void UFallHold::OnMovePressed(FVector2D InMoveInput)
{
	FVector Dir = Character->GetActorForwardVector() * InMoveInput.Y + Character->GetActorRightVector() * InMoveInput.X;
	Dir.Normalize();
	Character->AddMovementInput(Dir);
}

void UFallHold::OnThrowingStarted()
{
	StateMachine->ChangeState(EState::FallThrow);
}

void UFallHold::OnReleaseStarted()
{
	StateMachine->ChangeState(EState::FallRelease);
}

void UFallHold::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UFallHold::OnMovePressed);
	Character->OnThrowingStarted.AddDynamic(this,&UFallHold::OnThrowingStarted);
	Character->OnGrabbingStarted.AddDynamic(this,&UFallHold::OnReleaseStarted);
	
}

void UFallHold::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UFallHold::OnMovePressed);
	Character->OnThrowingStarted.RemoveDynamic(this,&UFallHold::OnThrowingStarted);
	Character->OnGrabbingStarted.RemoveDynamic(this,&UFallHold::OnReleaseStarted);
}
