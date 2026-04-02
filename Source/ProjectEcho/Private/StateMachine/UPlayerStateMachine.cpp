#include "StateMachine/UPlayerStateMachine.h"

#include "EnhancedInputComponent.h"
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

void UPlayerStateMachine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	    
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
}
