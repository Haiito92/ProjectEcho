#pragma once
#include "StateMachine/UState.h"
<<<<<<<< HEAD:Source/ProjectEcho/Public/StateMachine/States/UIdle.h
#include "UIdle.generated.h"

UCLASS()
class UIdle : public UState
========
#include "URevive.generated.h"

UCLASS()
class URevive : public UState
>>>>>>>> origin/Level-Design:Source/ProjectEcho/Public/StateMachine/States/URevive.h
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	virtual bool CanUseGrab() override;
	virtual bool CanUseRecord() override;
	virtual bool CanUseInteract() override;
	
private:
<<<<<<<< HEAD:Source/ProjectEcho/Public/StateMachine/States/UIdle.h

	virtual void OnMovePressed(FVector2D InMoveInput) override;
========
>>>>>>>> origin/Level-Design:Source/ProjectEcho/Public/StateMachine/States/URevive.h
};
