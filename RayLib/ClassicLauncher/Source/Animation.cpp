#include "Animation.h"
#include "External/reasings.h"

Animation::Animation()
	: currentTime(0)
	, duration(0)
	, bStart(false)
	, bFinish(false)
	, bReset(false)
	, bRelative(false)
	, type(Ease::EaseLinearNone)
{
}

void Animation::StartAnimation(const int durationAnimation, const Position& startAnimationPosition, const Position& finalAnimationPosition, const Ease::Ease typeAnimation, const bool bForceReset)
{
	bFinish = false;
	bStart = true;
	currentTime = 0;
	duration = durationAnimation;
	currentPosition = startAnimationPosition;
	startPosition = startAnimationPosition;
	finalPosition = finalAnimationPosition;
	bReset = bForceReset;
	type = typeAnimation;
}

void Animation::UpdateAnimation()
{
	bFinish = false;
	if (bStart)
	{
		if (currentTime <= duration)
		{
			currentPosition.position.x = GetAnimation(static_cast<float>(currentTime), startPosition.position.x, finalPosition.position.x - startPosition.position.x, static_cast<float>(duration));
			currentPosition.position.y = GetAnimation(static_cast<float>(currentTime), startPosition.position.y, finalPosition.position.y - startPosition.position.y, static_cast<float>(duration));
			currentPosition.scale.x = GetAnimation(static_cast<float>(currentTime), startPosition.scale.x, finalPosition.scale.x - startPosition.scale.x, static_cast<float>(duration));
			currentPosition.scale.y = GetAnimation(static_cast<float>(currentTime), startPosition.scale.y, finalPosition.scale.y - startPosition.scale.y, static_cast<float>(duration));


			const float r = GetAnimation(
				static_cast<float>(currentTime), 
				static_cast<float>(startPosition.color.GetColorRed()), 
				static_cast<float>(finalPosition.color.GetColorRed() - startPosition.color.GetColorRed()),
				static_cast<float>(duration)
			);
			const float g = GetAnimation(
				static_cast<float>(currentTime),
				static_cast<float>(startPosition.color.GetColorGreen()),
				static_cast<float>(finalPosition.color.GetColorGreen() - startPosition.color.GetColorGreen()),
				static_cast<float>(duration)
			);
			const float b = GetAnimation(
				static_cast<float>(currentTime),
				static_cast<float>(startPosition.color.GetColorBlue()),
				static_cast<float>(finalPosition.color.GetColorBlue() - startPosition.color.GetColorBlue()),
				static_cast<float>(duration)
			);
			const float a = GetAnimation(
				static_cast<float>(currentTime),
				static_cast<float>(startPosition.color.GetColorAlpha()),
				static_cast<float>(finalPosition.color.GetColorAlpha() - startPosition.color.GetColorAlpha()),
				static_cast<float>(duration)
			);

			currentPosition.color.SetColor(r, g, b, a);

			currentTime++;
			return;
		}

		FinishAnimation();
	}
}

void Animation::FinishAnimation()
{
	bFinish = true;
	ResetAnimation();
}

void Animation::ResetAnimation()
{
	bStart = false;
	currentPosition = (bReset) ? startPosition : finalPosition;
	currentTime = 0;
}

bool Animation::GetAnimationIsRun() const
{
	return bStart;
}

bool Animation::GetAnimationFinish() const
{
	return bFinish;
}

float Animation::GetAnimation(const float t, const float b, const float c, const float d) const
{
	switch (type) {
		case Ease::EaseLinearNone: return EaseLinearNone(t, b, c, d);
		case Ease::EaseLinearIn: return EaseLinearIn(t, b, c, d);
		case Ease::EaseLinearOut: return EaseLinearOut(t, b, c, d);
		case Ease::EaseLinearInOut: return EaseLinearInOut(t, b, c, d);
		case Ease::EaseSineIn: return EaseSineIn(t, b, c, d);
		case Ease::EaseSineOut: return EaseSineOut(t, b, c, d);
		case Ease::EaseSineInOut: return EaseSineInOut(t, b, c, d);
		case Ease::EaseCircIn: return EaseCircIn(t, b, c, d);
		case Ease::EaseCircOut: return EaseCircOut(t, b, c, d);
		case Ease::EaseCircInOut: return EaseCircInOut(t, b, c, d);
		case Ease::EaseCubicIn: return EaseCubicIn(t, b, c, d);
		case Ease::EaseCubicOut: return EaseCubicOut(t, b, c, d);
		case Ease::EaseCubicInOut: return EaseCubicInOut(t, b, c, d);
		case Ease::EaseQuadIn: return EaseQuadIn(t, b, c, d);
		case Ease::EaseQuadOut: return EaseQuadOut(t, b, c, d);
		case Ease::EaseQuadInOut: return EaseQuadInOut(t, b, c, d);
		case Ease::EaseExpoIn: return EaseExpoIn(t, b, c, d);
		case Ease::EaseExpoOut: return EaseExpoOut(t, b, c, d);
		case Ease::EaseExpoInOut: return EaseExpoInOut(t, b, c, d);
		case Ease::EaseBackIn: return EaseBackIn(t, b, c, d);
		case Ease::EaseBackOut: return EaseBackOut(t, b, c, d);
		case Ease::EaseBackInOut: return EaseBackInOut(t, b, c, d);
		case Ease::EaseBounceOut: return EaseBounceOut(t, b, c, d);
		case Ease::EaseBounceIn: return EaseBounceIn(t, b, c, d);
		case Ease::EaseBounceInOut: return EaseBounceInOut(t, b, c, d);
		case Ease::EaseElasticIn: return EaseElasticIn(t, b, c, d);
		case Ease::EaseElasticOut: return EaseElasticOut(t, b, c, d);
		case Ease::EaseElasticInOut: return EaseElasticInOut(t, b, c, d);
	}
	return EaseLinearNone(t, b, c, d);
}
