#include "StateMachine/States/UJump.h"

#include "EchoSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordHandlerComponent.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/ACharacterST.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"


void UJump::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	Super::InitState(InStateMachine, InCharacter);
	EnumState = EState::Jump;
	if (Character != nullptr)
		Character->GetCharacterMovement()->JumpZVelocity = JumpForce;
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