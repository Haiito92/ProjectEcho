#pragma once
#include "StateMachine/UState.h"
#include "UWalk.generated.h"

UCLASS()
class UWalk : public UState
{
public:
	GENERATED_BODY()
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed = 600;
	
	EState EnumState = EState::Walk;
private:
	
	UFUNCTION()
	void OnRunningStarted(bool InRunningStarted);
};
