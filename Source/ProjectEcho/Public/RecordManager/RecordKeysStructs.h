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
	StartPropulse,
	StopPropulse,
	TryReflect,
	StartReflect,
	StopReflect,
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
	
	bool operator==(const FRecordedAction& Other) const
	{
		return ActionEnum == Other.ActionEnum;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERecordedAction ActionEnum = ERecordedAction::None;
};

//Key used to save the position of an element at a set timekey 
USTRUCT(BlueprintType)
struct FRecordTransformKey
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ControlRotation;
};

USTRUCT(BlueprintType)
struct FRecordActionKey
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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