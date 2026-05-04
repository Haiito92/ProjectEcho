#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "UUserWidgetTimeline.generated.h"

USTRUCT(BlueprintType)
struct FTimelineData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float StartTime;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float EndTime;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FColor Color;
};

UCLASS(BlueprintType)
class PROJECTECHO_API UUserWidgetTimeline : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FTimelineData> TimelineUI;
private:
	URecordManagerSubsystem* RecordManagerSubsystem;
};
	
	
	
	
