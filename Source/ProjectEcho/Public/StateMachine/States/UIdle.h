#pragma once
#include "StateMachine/UState.h"
#include "UIdle.generated.h"

UCLASS()
class UIdle : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
private:
	UFUNCTION()
	void OnMove(FVector2D InMoveInput);
	
	UPROPERTY()
	FVector2D MoveInput;
};
