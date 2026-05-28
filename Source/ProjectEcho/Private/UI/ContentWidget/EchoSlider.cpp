// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContentWidget/EchoSlider.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/DefaultStyleCache.h"
#include "UI/ContentWidget/SEchoSlider.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// USlider

UEchoSlider::UEchoSlider(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	MinValue = 0.0f;
	MaxValue = 1.0f;
	Orientation = EOrientation::Orient_Horizontal;
	SliderBarColor = FLinearColor::White;
	SliderHandleColor = FLinearColor::White;
	StepSize = 0.01f;
	IsFocusable = true;
	MouseUsesStep = false;
	RequiresControllerLock = true;

	WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetSliderStyle();
	
#if WITH_EDITOR 
	if (IsEditorWidget())
	{
		WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetSliderStyle();
	}
#endif // WITH_EDITOR

PRAGMA_ENABLE_DEPRECATION_WARNINGS

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

TSharedRef<SWidget> UEchoSlider::RebuildWidget()
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	MySlider = SNew(SEchoSlider)
		.Style(&WidgetStyle)
		.IsFocusable(IsFocusable)
		.OnMouseCaptureBegin(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnMouseCaptureBegin))
		.OnMouseCaptureEnd(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnMouseCaptureEnd))
		.OnControllerCaptureBegin(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnControllerCaptureBegin))
		.OnControllerCaptureEnd(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnControllerCaptureEnd))
		.OnReceivedFocus_UObject(this, &ThisClass::SlateHandleOnReceivedFocus)
		.OnLostFocus_UObject(this, &ThisClass::SlateHandleOnLostFocus)
		.OnValueChanged(BIND_UOBJECT_DELEGATE(FOnFloatValueChanged, HandleOnValueChanged))
		.PreventThrottling(bPreventThrottling);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	return MySlider.ToSharedRef();
}

void UEchoSlider::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (!MySlider.IsValid())
	{
		return;
	}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	TAttribute<float> ValueBinding = PROPERTY_BINDING(float, Value);
	
	MySlider->SetOrientation(Orientation);
	MySlider->SetMouseUsesStep(MouseUsesStep);
	MySlider->SetRequiresControllerLock(RequiresControllerLock);
	MySlider->SetSliderBarColor(SliderBarColor);
	MySlider->SetSliderHandleColor(SliderHandleColor);
	MySlider->SetValue(ValueBinding);
	MySlider->SetMinAndMaxValues(MinValue, MaxValue);
	MySlider->SetLocked(Locked);
	MySlider->SetIndentHandle(IndentHandle);
	MySlider->SetStepSize(StepSize);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UEchoSlider::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySlider.Reset();
}

void UEchoSlider::HandleOnValueChanged(float InValue)
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Value = InValue;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	OnValueChanged.Broadcast(InValue);
	BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::Value);
}

void UEchoSlider::HandleOnMouseCaptureBegin()
{
	OnMouseCaptureBegin.Broadcast();
}

void UEchoSlider::HandleOnMouseCaptureEnd()
{
	OnMouseCaptureEnd.Broadcast();
}

void UEchoSlider::HandleOnControllerCaptureBegin()
{
	OnControllerCaptureBegin.Broadcast();
}

void UEchoSlider::HandleOnControllerCaptureEnd()
{
	OnControllerCaptureEnd.Broadcast();
}

void UEchoSlider::SlateHandleOnReceivedFocus()
{
	ReceivedFocus();
	OnReceivedFocus.Broadcast();
}

void UEchoSlider::SlateHandleOnLostFocus()
{
	LostFocus();
	OnLostFocus.Broadcast();
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS

float UEchoSlider::GetValue() const
{
	if ( MySlider.IsValid() )
	{
		return MySlider->GetValue();
	}

	return Value;
}

float UEchoSlider::GetNormalizedValue() const
{
	if (MySlider.IsValid())
	{
		return MySlider->GetNormalizedValue();
	}

	if (MinValue == MaxValue)
	{
		return 1.0f;
	}
	else
	{
		return (Value - MinValue) / (MaxValue - MinValue);
	}
}

void UEchoSlider::SetValue(float InValue)
{
	if (MySlider.IsValid())
	{
		MySlider->SetValue(InValue);
	}

	if (Value != InValue)
	{
		Value = InValue;
		HandleOnValueChanged(InValue);
	}
}

float UEchoSlider::GetMinValue() const
{
	if (MySlider.IsValid())
	{
		return MySlider->GetMinValue();
	}
	return MinValue;
}

void UEchoSlider::SetMinValue(float InValue)
{
	MinValue = InValue;
	if (MySlider.IsValid())
	{
		// Because SSlider clamps min/max values upon setting them,
		// we have to send both values together to ensure that they
		// don't get out of sync.
		MySlider->SetMinAndMaxValues(MinValue, MaxValue);
	}
}

float UEchoSlider::GetMaxValue() const
{
	if (MySlider.IsValid())
	{
		return MySlider->GetMaxValue();
	}
	return MaxValue;
}

void UEchoSlider::SetMaxValue(float InValue)
{
	MaxValue = InValue;
	if (MySlider.IsValid())
	{
		MySlider->SetMinAndMaxValues(MinValue, MaxValue);
	}
}

const FSliderStyle& UEchoSlider::GetWidgetStyle() const
{
	return WidgetStyle;
}

void UEchoSlider::SetWidgetStyle(const FSliderStyle& InStyle)
{
	WidgetStyle = InStyle;
	if (MySlider.IsValid())
	{
		MySlider->SetStyle(&WidgetStyle);
	}
}

EOrientation UEchoSlider::GetOrientation() const
{
	return Orientation;
}

void UEchoSlider::SetOrientation(EOrientation InOrientation)
{
	Orientation = InOrientation;
	if (MySlider.IsValid())
	{
		MySlider->SetOrientation(Orientation);
	}
}

bool UEchoSlider::HasIndentHandle() const
{
	return IndentHandle;
}

void UEchoSlider::SetIndentHandle(bool InIndentHandle)
{
	IndentHandle = InIndentHandle;
	if ( MySlider.IsValid() )
	{
		MySlider->SetIndentHandle(InIndentHandle);
	}
}

bool UEchoSlider::IsLocked() const
{
	return Locked;
}

void UEchoSlider::SetLocked(bool InLocked)
{
	Locked = InLocked;
	if ( MySlider.IsValid() )
	{
		MySlider->SetLocked(InLocked);
	}
}

float UEchoSlider::GetStepSize() const
{
	if (MySlider.IsValid())
	{
		return MySlider->GetStepSize();
	}
	return StepSize;
}

void UEchoSlider::SetStepSize(float InValue)
{
	StepSize = InValue;
	if (MySlider.IsValid())
	{
		MySlider->SetStepSize(InValue);
	}
}

FLinearColor UEchoSlider::GetSliderHandleColor() const
{
	return SliderHandleColor;
}

void UEchoSlider::SetSliderHandleColor(FLinearColor InValue)
{
	SliderHandleColor = InValue;
	if (MySlider.IsValid())
	{
		MySlider->SetSliderHandleColor(InValue);
	}
}

FLinearColor UEchoSlider::GetSliderBarColor() const
{
	return SliderBarColor;
}

void UEchoSlider::SetSliderBarColor(FLinearColor InValue)
{
	SliderBarColor = InValue;
	if (MySlider.IsValid())
	{
		MySlider->SetSliderBarColor(InValue);
	}
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

#if WITH_ACCESSIBILITY
void UEchoSlider::ReceivedFocus_Implementation()
{
}

void UEchoSlider::LostFocus_Implementation()
{
}

TSharedPtr<SWidget> UEchoSlider::GetAccessibleWidget() const
{
	return MySlider;
}
#endif

#if WITH_EDITOR

const FText UEchoSlider::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
