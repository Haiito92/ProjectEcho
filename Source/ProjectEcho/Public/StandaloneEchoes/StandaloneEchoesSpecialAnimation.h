#pragma once

#include "CoreMinimal.h"
#include "RecordManager/RecordKeysStructs.h"
#include "StandaloneEchoesSpecialAnimation.generated.h"

USTRUCT(BlueprintType)
struct FStandaloneEchoesSpecialAnimation
{
	GENERATED_BODY()
	
	FStandaloneEchoesSpecialAnimation()
	{
	}
	
	FStandaloneEchoesSpecialAnimation(EAnimationValueReference InAnimationValueReference, const float& LocalTimeKey)
	{
		AnimationValueReference = InAnimationValueReference;
		TimeKey = LocalTimeKey;
	}
	
	UPROPERTY(BlueprintReadWrite)
	EAnimationValueReference AnimationValueReference = EAnimationValueReference::None;
	
	UPROPERTY(BlueprintReadWrite)
	float TimeKey = 0.f;
};

//Unreal can't store Array of struct in TMap apparently, so we make a Wrapper
USTRUCT(BlueprintType)
struct FStandaloneEchoesAnimationArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStandaloneEchoesSpecialAnimation> Animations;
};