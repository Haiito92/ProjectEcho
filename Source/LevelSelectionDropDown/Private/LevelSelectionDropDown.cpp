#include "LevelSelectionDropDown.h"

#define LOCTEXT_NAMESPACE "FLevelSelectionDropDownModule"

void FLevelSelectionDropDownModule::StartupModule()
{
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
        this, &FLevelSelectionDropDownModule::RegisterMenuExtensions));
}

void FLevelSelectionDropDownModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
}

void FLevelSelectionDropDownModule::RegisterMenuExtensions()
{
    FToolMenuOwnerScoped OwnerScoped(this);
    
    UToolMenu* SecondaryToolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.SecondaryToolbar");
    if (!SecondaryToolbar) return;
    
    FToolMenuSection& DropDownSection = SecondaryToolbar->AddSection("LevelSelectionDropDown");
    DropDownSection.Label = LOCTEXT("LevelSelectionDropDown", "Level Selection Section"); 
    
    FToolMenuEntry ComboEntry = FToolMenuEntry::InitComboButton(
        "LevelSelectionCombo",
        FUIAction(),
        FNewToolMenuChoice(),
        LOCTEXT("LevelSelectionComboLabel", "Level Selection"),
        LOCTEXT("LevelSelectionComboTip", "Choose a level to open."),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings"));
    
    DropDownSection.AddEntry(ComboEntry);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLevelSelectionDropDownModule, LevelSelectionDropDown)