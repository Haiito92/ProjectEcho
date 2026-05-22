#pragma once

#include "CoreMinimal.h"
#include "EchoColorStruct.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FEchoColorStruct
{
	GENERATED_BODY()
	
public:
	//Echo Color
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::White;
	
	//Echo Material
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> Material = nullptr;
	
	//Post Process Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PostProcessIndex = 0;
};