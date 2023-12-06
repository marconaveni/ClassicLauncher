// Copyright 2023 Marco Naveni. All Rights Reserved.


#include "AnimationUI.h"
#include "Components/Widget.h"


void UAnimationUI::PlayAnimation(UWidget* Target, float Time, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve, bool ForceUpdateAnimation)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AnimationTimerHandle) && !ForceUpdateAnimation)
	{
		return;
	}
	EasingFunc = FunctionCurve;
	TimeAnimation = Time;
	RenderOpacity = ToOpacity;
	TargetPosition = ToPosition;
	bResetPosition = bReset;
	WidgetTarget = Target;
	FramesPerSeconds = GetWorld()->GetDeltaSeconds() / 2;
	InitialRenderOpacity = Target->GetRenderOpacity();
	InitialPosition = Target->GetRenderTransform();

	OnStartAnimationTrigger.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UAnimationUI::Animation, FramesPerSeconds, false, -1);
	
}

void UAnimationUI::Animation()
{
	Alpha = ((static_cast<float>(1) / TimeAnimation) * GetWorld()->GetDeltaSeconds()) + Alpha;
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (Alpha >= 1)
	{
		Alpha = 1;
	}


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

	if (Alpha >= 1)
	{
		if (bResetPosition)
		{
			WidgetTarget->SetRenderTransform(InitialPosition);
			WidgetTarget->SetRenderOpacity(InitialRenderOpacity);
		}

		Alpha = 0;
		CurrentTime = 0;
		GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
		AnimationTimerHandle.Invalidate();
		OnFinishAnimationTrigger.Broadcast();
		return;
	}
	FramesPerSeconds = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UAnimationUI::Animation, FramesPerSeconds / 2, false, -1);

}

float UAnimationUI::SetPositionsCurves(const UCurveFloat* Value) const
{
	return Value->GetFloatValue(Alpha);
}

float UAnimationUI::SetPositions(float ValueInitial, float ValueTarget, float ValueAlpha) const
{
	return UKismetMathLibrary::Ease(ValueInitial, ValueTarget, ValueAlpha, EasingFunc);
}

void UAnimationUI::SetCurves(const FAnimationUICurves& Curves)
{
	AnimationCurves = Curves;
}

void UAnimationUI::ClearAnimation()
{
	GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
}

float UAnimationUI::GetCurrentTimeAnimation() const
{
	return CurrentTime;
}
