#pragma once

#include <string>

#include "External/reasings.h"

namespace Ease
{
	enum
	{
		EaseLinearNone,
		EaseLinearIn,
		EaseLinearOut,
		EaseLinearInOut,
		EaseSineInne,
		EaseSineOut,
		EaseSineInOutIn,
		EaseCircInar,
		EaseCircOutlar,
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

	int currentTime = 0;
	int duration = 25;
	float startPositionX = 0.0f;
	float finalPositionX = 60.0f;
	float currentPositionX = startPositionX;

	bool bStart = false;
	bool bRelative;
	std::string name;

	Animation();


	void StartAnimation();
	void UpdateAnimation();
	void FinishAnimation();

protected:

};
