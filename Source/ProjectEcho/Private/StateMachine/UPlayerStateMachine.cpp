#include "StateMachine/UPlayerStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "StateMachine/UStateMachine.h"

void UPlayerStateMachine::InitStates()
{
	Idle = NewObject<UIdle>(this);
	Walk = NewObject<UWalk>(this);
	Run = NewObject<URun>(this);
	Jump = NewObject<UJump>(this);
	Fall = NewObject<UFall>(this);
	WallRun = NewObject<UWallRun>(this);
	
	Idle->InitStateMachine(this);
	Run->InitStateMachine(this);
	Jump->InitStateMachine(this);
	Fall->InitStateMachine(this);
	WallRun->InitStateMachine(this);
	Walk->InitStateMachine(this);
	
	AddState(Idle,"Idle");
	AddState(Walk,"Walk");
	AddState(Run,"Run");
	AddState(Jump,"Jump");
	AddState(Fall,"Fall");
	AddState(WallRun,"WallRun");
	
	StartState("Idle");
}

void UPlayerStateMachine::AIdle(const FInputActionValue& Value)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("en Idle"));
	if (CurrentState != Idle) ChangeState("Idle");
}

void UPlayerStateMachine::AMove(const FInputActionValue& Value)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("en Walk"));
	if (CurrentState != Walk) ChangeState("Walk");
	const FVector2D MovementValue = Value.Get<FVector2D>();
	
	if (PlayerController)
	{
		const FVector Right = Character->GetActorRightVector();
		Character->AddMovementInput(Right, MovementValue.X);
		
		const FVector Forward = Character->GetActorForwardVector();
		Character->AddMovementInput(Forward, MovementValue.Y);
	}
}

void UPlayerStateMachine::ASprint(const FInputActionValue& Value)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("en Sprint"));
	if (CurrentState != Run) ChangeState("Sprint");
}


void UPlayerStateMachine::AJump(const FInputActionValue& Value)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("en Jump"));
	if (CurrentState != Jump) ChangeState("Jump");
}



