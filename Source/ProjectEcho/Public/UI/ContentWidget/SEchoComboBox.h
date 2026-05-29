// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<typename OptionType>
class PROJECTECHO_API SEchoComboBox : public SComboBox<OptionType>
{
	
public:
	
	typedef TListTypeTraits< OptionType > ListTypeTraits;
	typedef typename TListTypeTraits< OptionType >::NullableType NullableOptionType;

	/** Type of list used for showing menu options. */
	typedef SListView< OptionType > SComboListType;
	/** Delegate type used to generate widgets that represent Options */
	typedef typename TSlateDelegates< OptionType >::FOnGenerateWidget FOnGenerateWidget;
	typedef typename TSlateDelegates< NullableOptionType >::FOnSelectionChanged FOnSelectionChanged;
	
	SLATE_BEGIN_ARGS( SEchoComboBox )
		: _Content()
		, _ComboBoxStyle(&FAppStyle::Get().GetWidgetStyle< FComboBoxStyle >("ComboBox"))
		, _ButtonStyle(nullptr)
		, _ItemStyle(&FAppStyle::Get().GetWidgetStyle< FTableRowStyle >("ComboBox.Row"))
		, _ScrollBarStyle(&FAppStyle::Get().GetWidgetStyle<FScrollBarStyle>("ScrollBar"))
		, _ContentPadding(_ComboBoxStyle->ContentPadding)
		, _ForegroundColor(FSlateColor::UseStyle())
		, _OnSelectionChanged()
		, _OnGenerateWidget()
		, _InitiallySelectedItem(ListTypeTraits::MakeNullPtr())
		, _Method()
		, _MaxListHeight(450.0f)
		, _HasDownArrow( true )
		, _EnableGamepadNavigationMode(false)
		, _IsFocusable( true )
		{}
		
		/** Slot for this button's content (optional) */
		SLATE_DEFAULT_SLOT( FArguments, Content )

		SLATE_STYLE_ARGUMENT( FComboBoxStyle, ComboBoxStyle )

		/** The visual style of the button part of the combo box (overrides ComboBoxStyle) */
		SLATE_STYLE_ARGUMENT( FButtonStyle, ButtonStyle )

		SLATE_STYLE_ARGUMENT(FTableRowStyle, ItemStyle)
		
		SLATE_STYLE_ARGUMENT( FScrollBarStyle, ScrollBarStyle )

		SLATE_ATTRIBUTE( FMargin, ContentPadding )
		SLATE_ATTRIBUTE( FSlateColor, ForegroundColor )

		SLATE_ITEMS_SOURCE_ARGUMENT( OptionType, OptionsSource )
		SLATE_EVENT( FOnSelectionChanged, OnSelectionChanged )
		SLATE_EVENT( FOnGenerateWidget, OnGenerateWidget )

		/** Called when combo box is opened, before list is actually created */
		SLATE_EVENT( FOnComboBoxOpening, OnComboBoxOpening )

		SLATE_EVENT( FSimpleDelegate, OnReceivedFocus )

		SLATE_EVENT( FSimpleDelegate, OnLostFocus )
		
		/** The custom scrollbar to use in the ListView */
		SLATE_ARGUMENT(TSharedPtr<SScrollBar>, CustomScrollbar)

		/** The option that should be selected when the combo box is first created */
		SLATE_ARGUMENT( NullableOptionType, InitiallySelectedItem )

		SLATE_ARGUMENT( TOptional<EPopupMethod>, Method )

		/** The max height of the combo box menu */
		SLATE_ARGUMENT(float, MaxListHeight)

		/** The sound to play when the button is pressed (overrides ComboBoxStyle) */
		SLATE_ARGUMENT( TOptional<FSlateSound>, PressedSoundOverride )

		/** The sound to play when the selection changes (overrides ComboBoxStyle) */
		SLATE_ARGUMENT( TOptional<FSlateSound>, SelectionChangeSoundOverride )

		/**
		 * When false, the down arrow is not generated and it is up to the API consumer
		 * to make their own visual hint that this is a drop down.
		 */
		SLATE_ARGUMENT( bool, HasDownArrow )

		/**
		 *  When false, directional keys will change the selection. When true, ComboBox
		 *	must be activated and will only capture arrow input while activated.
		*/
		SLATE_ARGUMENT(bool, EnableGamepadNavigationMode)

		/** When true, allows the combo box to receive keyboard focus */
		SLATE_ARGUMENT( bool, IsFocusable )

		/** True if this combo's menu should be collapsed when our parent receives focus, false (default) otherwise */
		SLATE_ARGUMENT(bool, CollapseMenuOnParentFocus)
				
	SLATE_END_ARGS()
	
	void Construct( const FArguments& InArgs );

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

protected:
	FSimpleDelegate OnReceivedFocus;
	FSimpleDelegate OnLostFocus;
};

#include "SEchoComboBox.inl"
