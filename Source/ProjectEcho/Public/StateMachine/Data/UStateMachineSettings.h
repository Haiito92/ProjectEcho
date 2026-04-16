#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UStateMachineSettings.generated.h"

class UState;

UCLASS()
class PROJECTECHO_API UStateMachineSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TSubclassOf<UState>> InstantiateStates;
};
