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
	int WalkSpeed = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Data")
	int RunSpeed = 900;
};