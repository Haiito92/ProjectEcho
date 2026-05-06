#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UPlayerData.generated.h"

class UInputAction;

UCLASS()
class UPlayerData : public UDataAsset
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Data")
	int InitLife = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Data")
	int MaxVelocity = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Data")
	float MoveAcceleration = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Data")
	float AirPrecision = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Data")
	float GravityScale = 1.f;	
};