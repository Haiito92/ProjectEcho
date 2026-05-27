// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"
#include "EchoSlider.generated.h"

/**
 * 
 */
class SEchoSlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseCaptureBeginEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseCaptureEndEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerCaptureBeginEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerCaptureEndEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSliderReceivedFocusEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSliderLostFocusEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatValueChangedEvent, float, Value);

UCLASS()
class PROJECTECHO_API UEchoSlider : public UWidget
{
	GENERATED_BODY()
	
	UEchoSlider(const FObjectInitializer& ObjectInitializer);
	
public:
	UE_DEPRECATED(5.1, "Direct access to Value is deprecated. Please use the getter or setter.")
	/** The volume value to display. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, FieldNotify, BlueprintGetter="GetValue", BlueprintSetter="SetValue", Category="Appearance", meta=(UIMin="0", UIMax="1"))
	float Value;

	/** A bindable delegate to allow logic to drive the value of the widget */
	UPROPERTY()
	FGetFloat ValueDelegate;

	UE_DEPRECATED(5.1, "Direct access to MinValue is deprecated. Please use the getter or setter.")
	/** The minimum value the slider can be set to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetMinValue", Category="Appearance")
	float MinValue;

	UE_DEPRECATED(5.1, "Direct access to MaxValue is deprecated. Please use the getter or setter.")
	/** The maximum value the slider can be set to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetMaxValue", Category="Appearance")
	float MaxValue;

public:
	UE_DEPRECATED(5.1, "Direct access to WidgetStyle is deprecated. Please use the getter or setter.")
	/** The progress bar style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, Category="Style", meta=( DisplayName="Style" ))
	FSliderStyle WidgetStyle;

	UE_DEPRECATED(5.1, "Direct access to Orientation is deprecated. Please use the getter or setter.")
	/** The slider's orientation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, Category=Appearance)
	TEnumAsByte<EOrientation> Orientation;

	UE_DEPRECATED(5.1, "Direct access to SliderBarColor is deprecated. Please use the getter or setter.")
	/** The color to draw the slider bar in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetSliderBarColor", Category="Appearance")
	FLinearColor SliderBarColor;

	UE_DEPRECATED(5.1, "Direct access to SliderHandleColor is deprecated. Please use the getter or setter.")
	/** The color to draw the slider handle in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetSliderHandleColor", Category="Appearance")
	FLinearColor SliderHandleColor;

	UE_DEPRECATED(5.1, "Direct access to IndentHandle is deprecated. Please use the getter or setter.")
	/** Whether the slidable area should be indented to fit the handle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter="HasIndentHandle", Setter, BlueprintSetter="SetIndentHandle", Category="Appearance", AdvancedDisplay)
	bool IndentHandle;

	UE_DEPRECATED(5.1, "Direct access to Locked is deprecated. Please use the getter or setter.")
	/** Whether the handle is interactive or fixed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter="IsLocked", Setter, BlueprintSetter="SetLocked", Category="Appearance", AdvancedDisplay)
	bool Locked;

	/** Sets new value if mouse position is greater/less than half the step size. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, AdvancedDisplay)
	bool MouseUsesStep;

	/** Sets whether we have to lock input to change the slider value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, AdvancedDisplay)
	bool RequiresControllerLock;

	UE_DEPRECATED(5.1, "Direct access to StepSize is deprecated. Please use the getter or setter.")
	/** The amount to adjust the value by, when using a controller or keyboard */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetStepSize", Category="Appearance", meta=(UIMin="0", UIMax="1"))
	float StepSize;

	/** Should the slider be focusable? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	bool IsFocusable;

	/** If refresh requests for the viewport should happen for all value changes **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	bool bPreventThrottling = false;

public:

	/** Invoked when the mouse is pressed and a capture begins. */
	UPROPERTY(BlueprintAssignable, Category="Widget Event")
	FOnMouseCaptureBeginEvent OnMouseCaptureBegin;

	/** Invoked when the mouse is released and a capture ends. */
	UPROPERTY(BlueprintAssignable, Category="Widget Event")
	FOnMouseCaptureEndEvent OnMouseCaptureEnd;

	/** Invoked when the controller capture begins. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnControllerCaptureBeginEvent OnControllerCaptureBegin;

	/** Invoked when the controller capture ends. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnControllerCaptureEndEvent OnControllerCaptureEnd;

	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnSliderReceivedFocusEvent OnReceivedFocus;
	
	UPROPERTY(BlueprintAssignable, Category = "Widget Event")
	FOnSliderLostFocusEvent OnLostFocus;
	
	/** Called when the value is changed by slider or typing. */
	UPROPERTY(BlueprintAssignable, Category="Widget Event")
	FOnFloatValueChangedEvent OnValueChanged;

	/** Gets the current value of the slider. */
	UFUNCTION(BlueprintCallable, Category="Behavior")
	float GetValue() const;

	/** Get the current value scaled from 0 to 1 */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	float GetNormalizedValue() const;

	/** Sets the current value of the slider. */
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void SetValue(float InValue);

	/** Gets the minimum value of the slider. */
	float GetMinValue() const;

	/** Sets the minimum value of the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void SetMinValue(float InValue);

	/** Gets the maximum value of the slider. */
	float GetMaxValue() const;

	/** Sets the maximum value of the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void SetMaxValue(float InValue);

	/** Get the style used by the widget. */
	const FSliderStyle& GetWidgetStyle() const;

	/** Set the style used by the widget. */
	void SetWidgetStyle(const FSliderStyle& InStyle);

	/** Getg the slider's orientation. */
	EOrientation GetOrientation() const;

	/** Sets the slider's orientation. */
	void SetOrientation(EOrientation InOrientation);

	/** Gets if the slidable area should be indented to fit the handle. */
	bool HasIndentHandle() const;

	/** Sets if the slidable area should be indented to fit the handle. */
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void SetIndentHandle(bool InValue);

	/** Returns true when the handle is fixed. */
	bool IsLocked() const;

	/** Sets the handle to be interactive or fixed. */
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void SetLocked(bool InValue);

	/** Gets the amount to adjust the value by. */
	float GetStepSize() const;

	/** Sets the amount to adjust the value by, when using a controller or keyboard. */
	UFUNCTION(BlueprintCallable, Category="Behavior")
	void SetStepSize(float InValue);

	/** Gets the color of the slider bar. */
	FLinearColor GetSliderBarColor() const;

	/** Sets the color of the slider bar. */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetSliderBarColor(FLinearColor InValue);

	/** Gets the color of the handle bar */
	FLinearColor GetSliderHandleColor() const;

	/** Sets the color of the handle bar */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetSliderHandleColor(FLinearColor InValue);
	
	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	/** Native Slate Widget */
	TSharedPtr<SEchoSlider> MySlider;

	
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	void HandleOnValueChanged(float InValue);
	void HandleOnMouseCaptureBegin();
	void HandleOnMouseCaptureEnd();
	void HandleOnControllerCaptureBegin();
	void HandleOnControllerCaptureEnd();

	void SlateHandleOnReceivedFocus();
	void SlateHandleOnLostFocus();
	
	UFUNCTION(BlueprintNativeEvent)
	void ReceivedFocus();
	UFUNCTION(BlueprintNativeEvent)
	void LostFocus();
	
#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

	PROPERTY_BINDING_IMPLEMENTATION(float, Value);
};
