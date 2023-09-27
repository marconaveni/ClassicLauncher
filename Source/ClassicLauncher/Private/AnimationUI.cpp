// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "AnimationUI.h"
#include "Components/Widget.h"


void UAnimationUI::PlayAnimation(UWidget* Target, float Time, int32 FPS, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve)
{
	EasingFunc = FunctionCurve;
	RenderOpacity = ToOpacity;
	TargetPosition = ToPosition;
	bResetPosition = bReset;
	WidgetTarget = Target;
	FramesPerSeconds = static_cast<float>(1) / static_cast<float>(FPS);
	Frames = Time * FPS;
	InitialRenderOpacity = Target->GetRenderOpacity();
	InitialPosition = Target->RenderTransform;


	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UAnimationUI::Animation, FramesPerSeconds, false, -1);
}

void UAnimationUI::Animation()
{
	UE_LOG(LogTemp, Warning, TEXT("animando"));

	Alpha += static_cast<float>(1) / Frames;

	FWidgetTransform NewPosition;
	NewPosition.Translation.X = (AnimationCurves.CheckTranslationX()) ? AnimationCurves.TranslationX->GetFloatValue(Alpha) : SetPositions(InitialPosition.Translation.X, TargetPosition.Translation.X, Alpha);
	NewPosition.Translation.Y = (AnimationCurves.CheckTranslationY()) ? AnimationCurves.TranslationX->GetFloatValue(Alpha) : SetPositions(InitialPosition.Translation.Y, TargetPosition.Translation.Y, Alpha);
	NewPosition.Scale.X = (AnimationCurves.CheckScaleX()) ? AnimationCurves.ScaleX->GetFloatValue(Alpha) : SetPositions(InitialPosition.Scale.X, TargetPosition.Scale.X, Alpha);
	NewPosition.Scale.Y = (AnimationCurves.CheckScaleY()) ? AnimationCurves.ScaleX->GetFloatValue(Alpha) : SetPositions(InitialPosition.Scale.Y, TargetPosition.Scale.Y, Alpha);
	NewPosition.Shear.X = (AnimationCurves.CheckShearX()) ? AnimationCurves.ShearX->GetFloatValue(Alpha) : SetPositions(InitialPosition.Shear.X, TargetPosition.Shear.X, Alpha);
	NewPosition.Shear.Y = (AnimationCurves.CheckShearY()) ? AnimationCurves.ShearX->GetFloatValue(Alpha) : SetPositions(InitialPosition.Shear.Y, TargetPosition.Shear.Y, Alpha);
	NewPosition.Angle = (AnimationCurves.CheckAngle()) ? AnimationCurves.Angle->GetFloatValue(Alpha) : UKismetMathLibrary::Ease(InitialPosition.Angle, TargetPosition.Angle, Alpha, EasingFunc);
	const float NewOpacity = (AnimationCurves.CheckOpacity()) ? AnimationCurves.Opacity->GetFloatValue(Alpha) : UKismetMathLibrary::Ease(InitialRenderOpacity, RenderOpacity, Alpha, EasingFunc);

	WidgetTarget->SetRenderTransform(NewPosition);
	WidgetTarget->SetRenderOpacity(NewOpacity);
	FramesCount++;

	if(FramesCount >= Frames)
	{
		if(bResetPosition)
		{
			WidgetTarget->SetRenderTransform(InitialPosition);
			WidgetTarget->SetRenderOpacity(InitialRenderOpacity);
		}
		FramesCount = 0;
		Alpha = 0;
		GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
		AnimationTimerHandle.Invalidate();
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(AnimationTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UAnimationUI::Animation, FramesPerSeconds, false, -1);
		}
	}
}

float UAnimationUI::SetPositionsCurves(const UCurveFloat* Value) const
{
	return Value->GetFloatValue(Alpha);
}

float UAnimationUI::SetPositions(float ValueInitial, float ValueTarget, float ValueAlpha) const
{
	return UKismetMathLibrary::Ease(ValueInitial, ValueTarget, ValueAlpha, EasingFunc);
}

void UAnimationUI::SetCurves(const FAnimationIUCurves& Curves)
{
	AnimationCurves = Curves;
}

void UAnimationUI::ClearAnimations()
{
	FramesPerSeconds = -1;
	GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
}
