#pragma once

#include "CoreMinimal.h"
#include "EchoColorStruct.h"
#include "RecordKeysStructs.h"
#include "UIRecordStructs.generated.h"

USTRUCT(BlueprintType)
struct FUIActionKey
{
	GENERATED_BODY()
	
	FUIActionKey()
	{
		Action = ERecordedAction::None;
		TimeKey = -1;
	}
	
	FUIActionKey(float InTimeKey, ERecordedAction InAction)
	{
		Action = InAction;
		TimeKey = InTimeKey;
	}
	
	UPROPERTY(BlueprintReadOnly)
	ERecordedAction Action = ERecordedAction::None;
	
	UPROPERTY(BlueprintReadOnly)
	float TimeKey = -1;
};

USTRUCT(BlueprintType)
struct FTimelineUIInfo
{
	GENERATED_BODY()
	
	
	UPROPERTY(BlueprintReadOnly)
	int Index = -1;
	
	UPROPERTY(BlueprintReadOnly)
	float StartTimeKey = -1;
	
	UPROPERTY(BlueprintReadOnly)
	float Length = -1;
	
	UPROPERTY(BlueprintReadOnly)
	float EndTimeKey = StartTimeKey + Length;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FUIActionKey> ActionKeys = TArray<FUIActionKey>();
	
	UPROPERTY(BlueprintReadOnly)
	FEchoColorStruct EchoColorStruct = FEchoColorStruct();
};

USTRUCT(BlueprintType)
struct FGlobalTimelineUIInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FTimelineUIInfo> Timelines;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentTimeKey;
	
	UPROPERTY(BlueprintReadOnly)
	int SelectedTimelineIndex;
	
	UPROPERTY(BlueprintReadOnly)
	float Length;
	
	UPROPERTY(BlueprintReadOnly)
	int NbSlots;
};