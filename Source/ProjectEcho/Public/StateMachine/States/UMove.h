#pragma once
#include "StateMachine/UState.h"
#include "UMove.generated.h"

UCLASS()
class UMove : public UState
{
public:
	GENERATED_BODY()
	virtual void Tick(float DeltaTime) override;	
	virtual void Enter() override;
	virtual void Exit() override;
private:
	UFUNCTION()
	void OnMove(FVector2D InMoveInput);
};
