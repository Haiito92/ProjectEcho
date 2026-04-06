#include "Public/StateMachine/UState.h"

class UStateMachine;

UState::UState()
{
}

void UState::InitStateMachine(UStateMachine* InStateMachine,ACharacterST* InCharacter)
{
	StateMachine = InStateMachine;
	Character = InCharacter;
}

void UState::Enter()
{

}

void UState::Tick(float DeltaTime)
{
}

void UState::Exit()
{
}
