#include "StateMachine/States/UWalk.h"
#include "StateMachine/UPlayerStateMachine.h"

void UWalk::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	//if (StateMachine->MinimumSpeedToRun < StateMachine->CurrentSpeed) StateMachine->ChangeState("Run");
	//if (StateMachine->CurrentSpeed == 0)StateMachine->ChangeState("Idle");
}
