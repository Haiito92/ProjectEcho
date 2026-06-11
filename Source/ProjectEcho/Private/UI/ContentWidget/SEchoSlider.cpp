// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ContentWidget/SEchoSlider.h"

void SEchoSlider::Construct(const FArguments& InArgs)
{
	SSlider::Construct(SSlider::FArguments()
		.IndentHandle(InArgs._IndentHandle)
		.MouseUsesStep(InArgs._MouseUsesStep)
   		.RequiresControllerLock(InArgs._RequiresControllerLock)
   		.Locked(InArgs._Locked)
   		.Orientation(InArgs._Orientation)
   		.SliderBarColor(InArgs._SliderBarColor)
   		.SliderHandleColor(InArgs._SliderHandleColor)
   		.Style(InArgs._Style)
   		.StepSize(InArgs._StepSize)
   		.Value(InArgs._Value)
   		.MinValue(InArgs._MinValue)
   		.MaxValue(InArgs._MaxValue)
   		.IsFocusable(InArgs._IsFocusable)
   		.OnMouseCaptureBegin(InArgs._OnMouseCaptureBegin)
   		.OnMouseCaptureEnd(InArgs._OnMouseCaptureEnd)
   		.OnControllerCaptureBegin(InArgs._OnControllerCaptureBegin)
   		.OnControllerCaptureEnd(InArgs._OnControllerCaptureEnd)
   		.OnValueChanged(InArgs._OnValueChanged)
   		.PreventThrottling(InArgs._PreventThrottling)
	);
	
	OnReceivedFocus = InArgs._OnReceivedFocus;
	OnLostFocus = InArgs._OnLostFocus;
}

FReply SEchoSlider::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FReply Reply = SSlider::OnFocusReceived(MyGeometry, InFocusEvent);
	bControllerInputCaptured = true;
	OnReceivedFocus.ExecuteIfBound();
	
	return Reply;
}

void SEchoSlider::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SSlider::OnFocusLost(InFocusEvent);
	OnLostFocus.ExecuteIfBound();
}
