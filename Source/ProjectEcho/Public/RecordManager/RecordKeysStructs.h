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
};

//Key used to save the position of an element at a set timekey 
USTRUCT(Blueprintable)
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
	ERecordedAction Action;
};