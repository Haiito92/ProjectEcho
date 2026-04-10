#include "StateMachine/States/UJump.h"
#include "StateMachine/ACharacterST.h"

void UJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckIsFalling();
}


void UJump::Enter()
{
	Super::Enter();
	Character->Jump();
}

void UJump::Exit()
{
	Super::Exit();
}