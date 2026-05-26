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
};

USTRUCT(BlueprintType)
//Key to Register a change in Animation Value
struct FRecordAnimationKey
{
	GENERATED_BODY()
	
	FRecordAnimationKey()
	{
		this->TimeKey = 0;
		this->AnimationValueReference = EAnimationValueReference::None;
	}
	
	FRecordAnimationKey(EAnimationValueReference AnimationValueReference)
	{
		this->AnimationValueReference = AnimationValueReference;
	}
	
	virtual ~FRecordAnimationKey() = default;
	float TimeKey = 0;
	EAnimationValueReference AnimationValueReference = EAnimationValueReference::None;
	
	virtual bool GetBoolValue() const { return false; }
	
	virtual float GetFloatValue() const { return 0; }
};

USTRUCT()
struct FRecordAnimationBooleanKey : public FRecordAnimationKey
{
	GENERATED_BODY()
	
	FRecordAnimationBooleanKey() : FRecordAnimationKey()
	{
		this->BoolValue = false;
	}
	
	FRecordAnimationBooleanKey(EAnimationValueReference AnimationValueReference, bool Value) : FRecordAnimationKey(AnimationValueReference)
	{
		this->BoolValue = Value;
	}

	bool BoolValue = 0;
	
	virtual bool GetBoolValue() const override { return BoolValue; }
};

USTRUCT()
struct FRecordAnimationFloatKey : public FRecordAnimationKey
{
	GENERATED_BODY()
	
	FRecordAnimationFloatKey() : FRecordAnimationKey()
	{
		this->FloatValue = 0;
	}
	
	FRecordAnimationFloatKey(EAnimationValueReference AnimationValueReference, float Value) : FRecordAnimationKey(AnimationValueReference)
	{
		this->FloatValue = Value;
	}

	float FloatValue = 0;
	
	virtual float GetFloatValue() const override { return FloatValue; }
};

