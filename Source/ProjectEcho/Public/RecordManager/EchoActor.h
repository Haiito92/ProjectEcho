// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoColorStruct.h"
#include "EchoInterface.h"
#include "GrabMechanic/GrabberActorInterface.h"
#include "LaserMechanic/Laserizable.h"
#include "ReflectMechanic/ReflectComponent.h"

#include "EchoActor.generated.h"

class UGrabbingComponent;
struct FRecordedAction;
class IRecordableInterface;
enum class ERecordedAction : uint8;

USTRUCT(Blueprintable)
struct FEchoStateSnapshot
{
	GENERATED_BODY()
	
	FEchoStateSnapshot()
	{
		bIsReflecting = false;
	}

	explicit FEchoStateSnapshot(const bool bInIsReflecting)
	{
		bIsReflecting = bInIsReflecting;
	}
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsReflecting = false;
};

UCLASS()
class PROJECTECHO_API AEchoActor : public AActor, public IEchoInterface, public ILaserizable, public IGrabberActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEchoActor();
protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	void HandleActionKey(const FRecordedAction& Action);
	
	virtual void ForceRelease_Implementation() override;
	
	//Take a Snapshot of all States to Restore
	void TakeStateSnapshot();
	
	//Restore State from previously recorded State Snapshot
	void RestoreStateSnapshot();
	
	UFUNCTION()
	void SetControlRotation(const FRotator& ControlRotation);
	
	UFUNCTION()
	void InitEcho(const int& index, const FEchoColorStruct& EchoColor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveInitEcho(FEchoColorStruct EchoColor);
	
	void HandleRewindStarted(const float& CurrentTimeKey, bool bIsPlayerRewind);
	void HandleRewindStopped(const float& CurrentTimeKey, bool bIsPlayerRewind); 
	void HandleRecordStarted(const float& CurrentTimeKey);
	
	UFUNCTION()
	void OnTimelineDestroyed();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOnTimelineDestroyed();
	
	UFUNCTION()
	virtual void Laserize_Implementation() override;
	
	//Called to Get Control Rotation
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
	FRotator GetEchoControlRotation();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveHandleActionKey(ERecordedAction Action);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveSetControlRotation(const FRotator& ControlRotation);
	
	UFUNCTION(BlueprintImplementableEvent, meta=(AutoCreateRefTerm="CurrentTimeKey"))
	void ReceiveHandleRewindStarted(const float& CurrentTimeKey);
	
	UFUNCTION(BlueprintImplementableEvent, meta=(AutoCreateRefTerm="CurrentTimeKey"))
	void ReceiveHandleRewindStopped(const float& CurrentTimeKey);
	
	UFUNCTION(BlueprintCallable)
	void RegisterRecordable(TScriptInterface<IRecordableInterface> Recordable);
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoDestroyedSignature, int, EchoIndex);
	UPROPERTY(BlueprintAssignable)
	FOnEchoDestroyedSignature OnEchoDestroyed;

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegisterRecordable, TScriptInterface<IRecordableInterface>, Recordable);
	UPROPERTY(BlueprintAssignable)
	FOnRegisterRecordable OnRegisterRecordable;
	
	UPROPERTY(BlueprintReadOnly)
	int EchoIndex = 0;
	
	UPROPERTY(BlueprintReadWrite)
	FEchoStateSnapshot EchoStateSnapshot;
	
private:
	UPROPERTY()
	TObjectPtr<UGrabbingComponent> GrabbingComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UReflectComponent> ReflectComponent = nullptr;
};
