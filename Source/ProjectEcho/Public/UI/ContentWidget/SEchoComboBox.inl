// Fill out your copyright notice in the Description page of Project Settings.

template <typename OptionType>
void SEchoComboBox<OptionType>::Construct(const FArguments& InArgs)
{
	SComboBox<OptionType>::Construct(SComboBox<OptionType>::FArguments()
		.ComboBoxStyle(InArgs._ComboBoxStyle)
		.ButtonStyle(InArgs._ButtonStyle)
		.ItemStyle(InArgs._ItemStyle)
		.ScrollBarStyle(InArgs._ScrollBarStyle)
		.ContentPadding(InArgs._ContentPadding)
		.ForegroundColor(InArgs._ForegroundColor)
		.OnSelectionChanged(InArgs._OnSelectionChanged)
		.OnGenerateWidget(InArgs._OnGenerateWidget)
		.InitiallySelectedItem(InArgs._InitiallySelectedItem)
		.Method(InArgs._Method)
		.MaxListHeight(InArgs._MaxListHeight)
		.HasDownArrow( InArgs._HasDownArrow )
		.EnableGamepadNavigationMode(InArgs._EnableGamepadNavigationMode)
		.IsFocusable( InArgs._IsFocusable )
		.OptionsSource(InArgs.GetOptionsSource()) 
		[
			InArgs._Content.Widget
		]
	);
	
	OnReceivedFocus = InArgs._OnReceivedFocus;
	OnLostFocus = InArgs._OnLostFocus;
}

template <typename OptionType>
FReply SEchoComboBox<OptionType>::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FReply Reply = SComboBox<OptionType>::OnFocusReceived(MyGeometry, InFocusEvent);
	OnReceivedFocus.ExecuteIfBound();
	return Reply;
}

template <typename OptionType>
void SEchoComboBox<OptionType>::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SComboBox<OptionType>::OnFocusLost(InFocusEvent);
	OnLostFocus.ExecuteIfBound();
}
