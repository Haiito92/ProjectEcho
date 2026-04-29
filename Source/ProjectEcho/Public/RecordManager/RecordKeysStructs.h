#pragma once

#include "CoreMinimal.h"
#include "RecordKeysStructs.generated.h"

UENUM(BlueprintType)
enum class ERecordedAction : uint8
{
	None,
	Jump,
	Interact,
	TryGrab,
	TryRelease,
	TryThrow,
	TryPropulse,
	TryReflect,
	ForceGrab,
	ForceRelease
};

USTRUCT(BlueprintType)
struct FRecordedAction
{
	GENERATED_BODY()
	
	virtual ~FRecordedAction() {}
	
	FRecordedAction()
	{
	}

	explicit FRecordedAction(const ERecordedAction Action)
	{
		this->ActionEnum = Action;
	}
	
	ERecordedAction ActionEnum = ERecordedAction::None;
};

//Key used to save the position of an element at a set timekey 
USTRUCT(BlueprintType)
struct FRecordTransformKey
{
	GENERATED_BODY()
	
	float TimeKey;
	FVector Position;
	FRotator Rotation;
	FVector Scale;
	FRotator ControlRotation;
};

USTRUCT()
struct FRecordActionKey
{
	GENERATED_BODY()
	
	float TimeKey;
	FRecordedAction Action;
};

USTRUCT(BlueprintType)
struct FRecordPhysicsKey
{
	GENERATED_BODY()
	
	float TimeKey;
	FVector LinearVelocity;
	FVector AngularVelocity;
};

USTRUCT(BlueprintType)
//Key to Register an interaction with a RecorderActor
struct FRecordInteractionKey
{
	GENERATED_BODY()
	
	float TimeKey;
	int RecordTimelineIndex;
};