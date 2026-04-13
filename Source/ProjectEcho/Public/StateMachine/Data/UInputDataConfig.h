#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UInputDataConfig.generated.h"

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AGrab;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AThrow;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ARecord;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AIncrementSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ADecrementSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ADestroySlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AInteract;
};