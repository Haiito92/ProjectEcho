#pragma once

#include "Widgets/SCompoundWidget.h"

class FAssetRegistryModule;

class SLevelSelectionCombo : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelSelectionCombo) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	~SLevelSelectionCombo();
private:
	TSharedRef<SWidget> GetMenuContent();
	FText GetCurrentLevelName() const;
	
	void UpdateLevelArray(const FAssetRegistryModule& AssetRegistry);
	
	void OnAssetAdded(const FAssetData& AssetData);
	void OnAssetRenamed(const FAssetData& AssetData, const FString& String);
	void OnAssetRemoved(const FAssetData& AssetData);
	
	TArray<FAssetData> AllLevels;
};
