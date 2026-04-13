#include "StateMachine/States/URewind.h"

#include "StateMachine/ACharacterST.h"

void URewind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void URewind::Enter()
{
	Super::Enter();
	Character->GetMesh()->SetSimulatePhysics(false);
	Character->DeactivateCharacterInput();
}

void URewind::Exit()
{
	Super::Exit();
	Character->GetMesh()->SetSimulatePhysics(true);
	Character->ActivateCharacterInput();
}