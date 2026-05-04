// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoColorStruct.h"

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
	float MinRecordTime = 3.0f;
	
	//Max Time to Record (After which Record will be stopped) in Seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Record")
	float MaxRecordTime = 10.0f;
	
	//Time taken for Player to rewind after finishing a record
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Rewind")
	float PlayerRewindTime = 0.5f;
	
	//Time taken for Global Timeline to rewind to Start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Replay")
	float GlobalRewindTime = 1.f;
	
	//Time taken for Standalone Global Timeline to rewind to Start
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Replay")
	float StandaloneGlobalRewindTime = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Echo Actors")
	TMap<int, FEchoColorStruct> EchoColors;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Echo Actors")
	FEchoColorStruct StandaloneEchoColor;
};
