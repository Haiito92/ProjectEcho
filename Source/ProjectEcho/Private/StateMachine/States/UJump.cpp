#include "StateMachine/States/UJump.h"

#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordHandlerComponent.h"
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
	if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(MakeShared<FRecordedAction>(FRecordedAction(ERecordedAction::Jump)));
	
}

void UJump::Exit()
{
	Super::Exit();
}