// Copyright 2024 Marco Naveni. All Rights Reserved.


#include "UI/Animations/AnimationUI.h"
#include "Components/Widget.h"


void UAnimationUI::PlayAnimation(UWidget* Target, float Time, FWidgetTransform ToPosition, float ToOpacity, bool bReset, TEnumAsByte<EEasingFunc::Type> FunctionCurve, bool ForceUpdateAnimation, FName CurrentNameAnimation)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AnimationTimerHandle) && !ForceUpdateAnimation || Target == nullptr)
	{
		return;
	}

	if (NameAnimation.IsNone() && !CurrentNameAnimation.IsNone())
	{
		NameAnimation = CurrentNameAnimation;
	}

	Alpha = 0;
	CurrentTime = 0;

	EasingFunc = FunctionCurve;
	TimeAnimation = Time;
	RenderOpacity = ToOpacity;
	TargetPosition = ToPosition;
	bResetPosition = bReset;
	WidgetTarget = Target;
	FramesPerSeconds = GetWorld()->GetDeltaSeconds() / 2;
	InitialRenderOpacity = Target->GetRenderOpacity();
	InitialPosition = Target->GetRenderTransform();

	OnStartAnimationTrigger.Broadcast(this, NameAnimation);
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UAnimationUI::Animation, FramesPerSeconds, false, -1);

}

UAnimationUI::UAnimationUI()
	: FramesPerSeconds(0)
	, Alpha(0)
	, RenderOpacity(1)
	, InitialRenderOpacity(1)
	, bResetPosition(false)
	, WidgetTarget(nullptr)
	, EasingFunc(EEasingFunc::Linear)
    , CurrentTime(0)
	, TimeAnimation(0.2f)
	, NameAnimation(NAME_None)
{
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
		ClearAnimation();
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
	if (bResetPosition)
	{
		WidgetTarget->SetRenderTransform(InitialPosition);
		WidgetTarget->SetRenderOpacity(InitialRenderOpacity);
	}

	Alpha = 0;
	CurrentTime = 0;
	GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
	AnimationTimerHandle.Invalidate();
	OnFinishAnimationTrigger.Broadcast(this, NameAnimation);
}

float UAnimationUI::GetCurrentTimeAnimation() const
{
	return CurrentTime;
}
