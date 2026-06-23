#pragma once

#include "GrabbingRules.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FGrabbingRules
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelsToIgnore = TArray<TEnumAsByte<ECollisionChannel>>();
	
	//Make Cube Transparent when grabbed
	UPROPERTY(BlueprintReadOnly)
	bool bMakeTransparent = false;
};
