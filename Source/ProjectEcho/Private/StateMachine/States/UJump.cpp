#include "StateMachine/States/UJump.h"

#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordHandlerComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/ACharacterST.h"

UJump::UJump()
{
	EnumState = EState::Jump;
}

void UJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckIsFalling();
}


void UJump::Enter()
{
	Super::Enter();
	Character->Jump();
	if (IsValid(RecordHandlerComponent)) RecordHandlerComponent->RegisterActionInRecord(FRecordedAction(ERecordedAction::Jump));
	
}

void UJump::Exit()
{
	Super::Exit();
}