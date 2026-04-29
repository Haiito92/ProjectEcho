#include "StateMachine/States/UIdle.h"

#include "EchoSystem.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"


void UIdle::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
	CheckIsFalling();
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::PlayerStateMachine,EEchoMessageType::Log,"in idle");
}

void UIdle::Enter()
{
	Super::Enter();
	Character->OnMovePressed.AddDynamic(this,&UIdle::OnMovePressed);
	Character->OnJumpingStarted.AddDynamic(this,&UIdle::OnJumpingStarted);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMovePressed.RemoveDynamic(this,&UIdle::OnMovePressed);
	Character->OnJumpingStarted.RemoveDynamic(this,&UIdle::OnJumpingStarted);
}

void UIdle::InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter)
{
	Super::InitState(InStateMachine, InCharacter);
	EnumState = EState::Idle;
}

void UIdle::OnMovePressed(FVector2D dir)
{
	StateMachine->ChangeState(EState::Move);
}

