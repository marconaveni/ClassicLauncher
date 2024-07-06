#include "Animation.h"

Animation::Animation()
{
}

void Animation::StartAnimation()
{
	bStart = true;
}

void Animation::UpdateAnimation()
{
	if (bStart)
	{
		if (currentPositionX < finalPositionX)
		{
			currentPositionX = EaseQuadOut((float)currentTime, startPositionX, finalPositionX - startPositionX, (float)duration);
			currentTime++;
			return;
		}

		FinishAnimation();
	}
}

void Animation::FinishAnimation()
{
	bStart = false;
	currentPositionX = 0;
	currentTime = 0;
}
