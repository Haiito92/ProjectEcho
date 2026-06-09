#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UStateMachineSettings.generated.h"

class UState;
enum class EPlayerActionType: uint8;

UCLASS()
class PROJECTECHO_API UStateMachineSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TSubclassOf<UState>> InstantiateStates;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TMap<EPlayerActionType, bool> StartingLockedActions;
};
