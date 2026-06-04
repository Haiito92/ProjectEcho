#pragma once

#include "CoreMinimal.h"
#include "EchoColorStruct.generated.h"

UENUM(BlueprintType)
enum class EEchoColor : uint8
{
	Blue = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Purple = 4,
};

USTRUCT(BlueprintType, Blueprintable)
struct FEchoColorStruct
{
	GENERATED_BODY()
	
public:
	//Echo Color Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEchoColor ColorEnum = EEchoColor::Blue;
	
	//Echo Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::White;
	
	//Echo Footstep Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor FootstepColor = FColor::White;
	
	//Post Process Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PostProcessIndex = 0;
};