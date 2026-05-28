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
	TArray<FTimelineUIInfo> Timelines = TArray<FTimelineUIInfo>();
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentTimeKey = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int SelectedTimelineIndex = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float Length = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int NbSlots = 0;
};