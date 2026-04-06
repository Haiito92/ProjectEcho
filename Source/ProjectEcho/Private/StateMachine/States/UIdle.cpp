#include "StateMachine/States/UIdle.h"
#include "StateMachine/ACharacterST.h"
#include "StateMachine/UStateMachine.h"


void UIdle::Tick(float DeltaTime)
{
	UState::Tick(DeltaTime);
}

void UIdle::Enter()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Idle"));
	Super::Enter();
	Character->OnMoveInput.AddDynamic(this,&UIdle::OnMove);
}

void UIdle::Exit()
{
	Super::Exit();
	Character->OnMoveInput.RemoveDynamic(this,&UIdle::OnMove);
}

void UIdle::OnMove(FVector2D InMoveInput)
{
	if (InMoveInput == FVector2D::ZeroVector) return;
	StateMachine->ChangeState(EState::Move);
}

