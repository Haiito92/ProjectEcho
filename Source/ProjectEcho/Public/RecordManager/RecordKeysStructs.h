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
	float TimeKey = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ControlRotation = FRotator::ZeroRotator;
};

USTRUCT(BlueprintType)
struct FRecordActionKey
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeKey = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRecordedAction Action = FRecordedAction();
};

USTRUCT(BlueprintType)
struct FRecordPhysicsKey
{
	GENERATED_BODY()
	
	float TimeKey = 0;
	FVector LinearVelocity = FVector::ZeroVector;
	FVector AngularVelocity = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
//Key to Register an interaction with a RecorderActor
struct FRecordInteractionKey
{
	GENERATED_BODY()
	float TimeKey = 0;
	int RecordTimelineIndex = 0;
};

UENUM(BlueprintType)
enum class EAnimationValueReference : uint8
{
	None = 0,
	IsGrabbing = 1,
	IsReleasing = 2,
	IsThrowing = 3,
	IsJumping = 4,
	IsFalling = 5,
	IsDead = 6,
	IsReviving = 7,
	IsMoving = 8,
	Speed = 9,
	IsRecording = 10,
	IsReflect = 11,
	OnRevive = 12,
	IsHoldingReflect = 13,
	IsNotReflect = 14,
};

UENUM(BlueprintType)
enum class EAnimationValueType : uint8
{
	None = 0,
	Bool = 1,
	Float = 2,
};

USTRUCT(BlueprintType)
//Reference to an Animation Value
struct FRecordAnimationValue
{
	GENERATED_BODY()
	
	FRecordAnimationValue()
	{
		this->AnimationValueReference = EAnimationValueReference::None;
		this->BoolValue = false;
		this->FloatValue = 0;
	}
	
	FRecordAnimationValue(EAnimationValueReference AnimationValueReference, bool BoolValue)
	{
		this->AnimationValueReference = AnimationValueReference;
		AnimationValueType = EAnimationValueType::Bool;
		this->BoolValue = BoolValue;
		this->FloatValue = 0;
	}
	
	FRecordAnimationValue(EAnimationValueReference AnimationValueReference, float FloatValue)
	{
		this->AnimationValueReference = AnimationValueReference;
		AnimationValueType = EAnimationValueType::Float;
		this->FloatValue = FloatValue;
		this->BoolValue = false;
	}
	
	UPROPERTY(BlueprintReadWrite)
	EAnimationValueReference AnimationValueReference = EAnimationValueReference::None;
	
	UPROPERTY(BlueprintReadWrite)
	EAnimationValueType AnimationValueType = EAnimationValueType::None;
	
	UPROPERTY(BlueprintReadWrite)
	bool BoolValue = false;
	
	UPROPERTY(BlueprintReadWrite)
	float FloatValue = 0;
};

USTRUCT(BlueprintType)
//Key to Register a change in Animation Value
struct FRecordAnimationKey
{
	GENERATED_BODY()
	
	FRecordAnimationKey()
	{
		this->TimeKey = 0;
		this->RecordAnimationValue = FRecordAnimationValue();
	}
	
	FRecordAnimationKey(const FRecordAnimationValue& AnimationValue, const float& TimeKey)
	{
		this->TimeKey = TimeKey;
		this->RecordAnimationValue = AnimationValue;
	}
	
	FRecordAnimationValue RecordAnimationValue = FRecordAnimationValue();
	float TimeKey = 0;
};

