#include "StateMachine/States/URun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void URun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsRunning) StateMachine->ChangeState(EState::Walk);
}

void URun::Enter()
{
	Super::Enter();
	Character->OnRunning.AddDynamic(this,&URun::OnRunning);
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->RunSpeed;
}

void URun::Exit()
{
	Character->OnRunning.RemoveDynamic(this,&URun::OnRunning);
	Super::Exit();
}

void URun::OnRunning(bool InRunning)
{
	IsRunning = InRunning;
}

