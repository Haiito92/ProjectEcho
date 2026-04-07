#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataConfig.generated.h"

class UInputAction;

UCLASS()
class UInputDataConfig : public UDataAsset
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AMove;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ARun;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AJump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ALook;
};