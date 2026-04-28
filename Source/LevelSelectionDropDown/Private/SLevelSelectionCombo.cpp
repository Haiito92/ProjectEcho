#include "SLevelSelectionCombo.h"

#include "AssetRegistry/AssetRegistryModule.h"

void SLevelSelectionCombo::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SComboButton)
		.OnGetMenuContent(this, &SLevelSelectionCombo::GetMenuContent)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &SLevelSelectionCombo::GetCurrentLevelName)
		]
	];
	
	FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistry.Get().OnAssetAdded().AddRaw(this, &SLevelSelectionCombo::OnAssetAdded);
	AssetRegistry.Get().OnAssetRenamed().AddRaw(this, &SLevelSelectionCombo::OnAssetRenamed);
	AssetRegistry.Get().OnAssetRemoved().AddRaw(this, &SLevelSelectionCombo::OnAssetRemoved);

	UpdateLevelArray(AssetRegistry);
}

SLevelSelectionCombo::~SLevelSelectionCombo()
{
	FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistry.Get().OnAssetAdded().RemoveAll(this);
}

TSharedRef<SWidget> SLevelSelectionCombo::GetMenuContent()
{
	return SNew(SBox);
}

FText SLevelSelectionCombo::GetCurrentLevelName() const
{
	if (!GEditor) return FText::FromString("Empty");
    
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return FText::FromString("Empty");
    
	return FText::FromString(World->GetMapName());
}

void SLevelSelectionCombo::UpdateLevelArray(const FAssetRegistryModule& AssetRegistry)
{
	AssetRegistry.Get().GetAssetsByClass(FTopLevelAssetPath(TEXT("/Script/Engine"), TEXT("World")), AllLevels);
}

void SLevelSelectionCombo::OnAssetAdded(const FAssetData& AssetData)
{
}

void SLevelSelectionCombo::OnAssetRenamed(const FAssetData& AssetData, const FString& String)
{
}

void SLevelSelectionCombo::OnAssetRemoved(const FAssetData& AssetData)
{
}
