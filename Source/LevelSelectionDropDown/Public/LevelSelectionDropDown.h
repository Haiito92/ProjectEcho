#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FLevelSelectionDropDownModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
protected:
    UFUNCTION()
    void RegisterMenuExtensions();
};
