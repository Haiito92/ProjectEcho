#include "StateMachine/States/URun.h"
#include "StateMachine/UPlayerStateMachine.h"

void URun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (StateMachine->CurrentSpeed < StateMachine->MinimumSpeedToRun) StateMachine->ChangeState("Walk");
}
