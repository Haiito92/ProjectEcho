// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RecordManagerSettings.generated.h"

class AEchoActor;

/**
 * 
 */
UCLASS()
class PROJECTECHO_API URecordManagerSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//Actor of Echo Character that will be instantiated and used to display Character's Actions in Replays
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Echo Actors")
	TSubclassOf<AEchoActor> EchoActorClass;
	
	//Factor at which Time Dilatation will be multiplied when in Record
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Record")
	float TimeDilatationFactor = 0.7f;
	
	//Max Time to Record (After which Record will be stopped) in Seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Record")
	float MaxRecordTime = 5.0f;
};
