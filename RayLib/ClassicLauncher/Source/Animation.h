#pragma once

#include <string>
#include <vector>

#include "Object.h"


namespace Ease
{
	enum Ease
	{
		EaseLinearNone,
		EaseLinearIn,
		EaseLinearOut,
		EaseLinearInOut,
		EaseSineIn,
		EaseSineOut,
		EaseSineInOut,
		EaseCircIn,
		EaseCircOut,
		EaseCircInOut,
		EaseCubicIn,
		EaseCubicOut,
		EaseCubicInOut,
		EaseQuadIn,
		EaseQuadOut,
		EaseQuadInOut,
		EaseExpoIn,
		EaseExpoOut,
		EaseExpoInOut,
		EaseBackIn,
		EaseBackOut,
		EaseBackInOut,
		EaseBounceOut,
		EaseBounceIn,
		EaseBounceInOut,
		EaseElasticIn,
		EaseElasticOut,
		EaseElasticInOut

	};

}

class Animation
{
public:

	int currentTime;
	int duration;

	bool bStart;
	bool bFinish;
	bool bReset;
	bool bRelative;
	std::string name;

	Ease::Ease type;

	Position startPosition;
	Position currentPosition;
	Position finalPosition;

	Animation();

	//std::vector<Object*> objectsAnimation;
	void StartAnimation(int durationAnimation, const Position& startAnimationPosition, const Position& finalAnimationPosition, Ease::Ease typeAnimation = Ease::EaseLinearNone, bool bForceReset = true);
	void UpdateAnimation();
	void FinishAnimation();
	void ResetAnimation();
	bool GetAnimationIsRun() const;
	bool GetAnimationFinish() const;

protected:

	float GetAnimation(float t, float b, float c, float d) const;
};
