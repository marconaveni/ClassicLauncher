#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include "Core.h"

namespace ClassicLauncher
{

    enum class Ease 
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

    class Animation
    {
    public:

        float mCurrentTime;
        float mDuration;

        bool bStart;
        bool bRunning;
        bool bFinish;
        bool bReset;
        bool bRelative;

        Ease mType;

        TransformProperties mStartTransform;
        TransformProperties mCurrentTransform;
        TransformProperties mFinalTransform;

        Animation();

        void StartAnimation(float durationAnimation,
                            const TransformProperties& startAnimationTransform,
                            const TransformProperties& finalAnimationTransform,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool bForceReset = true);
        void UpdateAnimation();
        void FinishAnimation();
        void ResetAnimation();
        bool GetAnimationIsRun() const;
        bool GetAnimationFinish() const;

    protected:

        float GetAnimation(float t, float b, float c, float d) const;
    };

}  // namespace ClassicLauncher

#endif  // ANIMATION_H
