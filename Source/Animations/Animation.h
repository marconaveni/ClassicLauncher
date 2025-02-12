#ifndef ANIMATION_H
#define ANIMATION_H


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

        bool mIsStart;
        bool mIsRunning;
        bool mIsFinish;
        bool mIsReset;
        bool mRelative;

        Ease mType;

        Transform mStartTransform;
        Transform mCurrentTransform;
        Transform mFinalTransform;

        Animation();

        void StartAnimation(float durationAnimation,
                            const Transform& startAnimationTransform,
                            const Transform& finalAnimationTransform,
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
