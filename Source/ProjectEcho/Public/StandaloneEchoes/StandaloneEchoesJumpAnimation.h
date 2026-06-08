#pragma once

#include "CoreMinimal.h"
#include "StandaloneEchoesJumpAnimation.generated.h"

enum class ESpecialAnimation
{
	None,
	JumpAnimation,
	FallAnimation,
};

USTRUCT(BlueprintType, Blueprintable)
struct FStandaloneEchoesSpecialAnimation
{
	GENERATED_BODY()

	ESpecialAnimation SpecialAnimationEnum = ESpecialAnimation::None;
};

USTRUCT(BlueprintType, Blueprintable)
struct FStandaloneEchoesFallAnimation : public FStandaloneEchoesSpecialAnimation
{
	GENERATED_BODY()

	explicit FStandaloneEchoesFallAnimation(const float& FallTimKey, const float& LandTimeKey)
	{
		SpecialAnimationEnum = ESpecialAnimation::FallAnimation;
		StartFallAnimationTime = FallTimKey;
		StartLandAnimationTime = LandTimeKey;
	}
	
	//Time Key of Start Fall Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartFallAnimationTime = 0.f;
	
	//Time Key of Start Land Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartLandAnimationTime = 0.f;
};

USTRUCT(BlueprintType, Blueprintable)
struct FStandaloneEchoesJumpAnimation : public FStandaloneEchoesSpecialAnimation
{
	GENERATED_BODY()

	explicit FStandaloneEchoesJumpAnimation(const float& JumpTimeKey)
	{
		SpecialAnimationEnum = ESpecialAnimation::JumpAnimation;
		StartJumpAnimationTime = JumpTimeKey;
	}
	
public:
	//Time Key of Start Jump Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartJumpAnimationTime = 0.f;
	
	//Skip Falling Animation to directly land while 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLandWithoutFall = true;
	
	//Fall Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStandaloneEchoesFallAnimation FallAnimation = FStandaloneEchoesFallAnimation(0.f, 0.f);
	
	//Directly Land Time Key
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LandTimeKey = 0.f;
};