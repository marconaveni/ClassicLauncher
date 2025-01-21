#include "Animation.h"
#include "reasings/reasings.h"

namespace ClassicLauncher
{

    Animation::Animation()
        : mCurrentTime(0), mDuration(0), mIsStart(false), mIsRunning(false), mIsFinish(false), mIsReset(false), mRelative(false), mType(Ease::EaseLinearNone) {};

    void Animation::StartAnimation(const float durationAnimation,
                                   const TransformProperties& startAnimation,
                                   const TransformProperties& finalAnimation,
                                   const Ease typeAnimation,
                                   const bool bForceReset)
    {
        mIsRunning = true;
        mIsStart = true;
        mIsFinish = false;
        mCurrentTime = 0.0f;
        mDuration = durationAnimation / GetFrameTime();
        mStartTransform = startAnimation;
        mCurrentTransform = startAnimation;
        mFinalTransform = finalAnimation;
        mIsReset = bForceReset;
        mType = typeAnimation;
    }

    void Animation::UpdateAnimation()
    {
        mIsFinish = false;
        if (mIsRunning)
        {
            if (mCurrentTime <= mDuration)
            {
                mCurrentTransform.x = GetAnimation(mCurrentTime, mStartTransform.x, mFinalTransform.x - mStartTransform.x, mDuration);
                mCurrentTransform.y = GetAnimation(mCurrentTime, mStartTransform.y, mFinalTransform.y - mStartTransform.y, mDuration);
                mCurrentTransform.scaleX = GetAnimation(mCurrentTime, mStartTransform.scaleX, mFinalTransform.scaleX - mStartTransform.scaleX, mDuration);
                mCurrentTransform.scaleY = GetAnimation(mCurrentTime, mStartTransform.scaleY, mFinalTransform.scaleY - mStartTransform.scaleY, mDuration);
                mCurrentTransform.rotation = GetAnimation(mCurrentTime, mStartTransform.rotation, mFinalTransform.rotation - mStartTransform.rotation, mDuration);
				mCurrentTransform.color.r = GetAnimation(mCurrentTime, mStartTransform.color.r, mFinalTransform.color.r - mStartTransform.color.r, mDuration);
				mCurrentTransform.color.g = GetAnimation(mCurrentTime, mStartTransform.color.g, mFinalTransform.color.g - mStartTransform.color.g, mDuration);
				mCurrentTransform.color.b = GetAnimation(mCurrentTime, mStartTransform.color.b, mFinalTransform.color.b - mStartTransform.color.b, mDuration);
				mCurrentTransform.color.a = GetAnimation(mCurrentTime, mStartTransform.color.a, mFinalTransform.color.a - mStartTransform.color.a, mDuration);

                mCurrentTime++;
                return;
            }

            FinishAnimation();
        }
    }

    void Animation::FinishAnimation()
    {
        mIsFinish = true;
        ResetAnimation();
    }

    void Animation::ResetAnimation()
    {
        mIsRunning = false;
        mCurrentTransform = (mIsReset) ? mStartTransform : mFinalTransform;
        mCurrentTime = 0;
    }

    bool Animation::GetAnimationIsRun() const
    {
        return mIsRunning;
    }

    bool Animation::GetAnimationFinish() const
    {
        return mIsFinish;
    }

    float Animation::GetAnimation(const float t, const float b, const float c, const float d) const
    {
        switch (mType)
        {
            case Ease::EaseLinearNone:
                return EaseLinearNone(t, b, c, d);
            case Ease::EaseLinearIn:
                return EaseLinearIn(t, b, c, d);
            case Ease::EaseLinearOut:
                return EaseLinearOut(t, b, c, d);
            case Ease::EaseLinearInOut:
                return EaseLinearInOut(t, b, c, d);
            case Ease::EaseSineIn:
                return EaseSineIn(t, b, c, d);
            case Ease::EaseSineOut:
                return EaseSineOut(t, b, c, d);
            case Ease::EaseSineInOut:
                return EaseSineInOut(t, b, c, d);
            case Ease::EaseCircIn:
                return EaseCircIn(t, b, c, d);
            case Ease::EaseCircOut:
                return EaseCircOut(t, b, c, d);
            case Ease::EaseCircInOut:
                return EaseCircInOut(t, b, c, d);
            case Ease::EaseCubicIn:
                return EaseCubicIn(t, b, c, d);
            case Ease::EaseCubicOut:
                return EaseCubicOut(t, b, c, d);
            case Ease::EaseCubicInOut:
                return EaseCubicInOut(t, b, c, d);
            case Ease::EaseQuadIn:
                return EaseQuadIn(t, b, c, d);
            case Ease::EaseQuadOut:
                return EaseQuadOut(t, b, c, d);
            case Ease::EaseQuadInOut:
                return EaseQuadInOut(t, b, c, d);
            case Ease::EaseExpoIn:
                return EaseExpoIn(t, b, c, d);
            case Ease::EaseExpoOut:
                return EaseExpoOut(t, b, c, d);
            case Ease::EaseExpoInOut:
                return EaseExpoInOut(t, b, c, d);
            case Ease::EaseBackIn:
                return EaseBackIn(t, b, c, d);
            case Ease::EaseBackOut:
                return EaseBackOut(t, b, c, d);
            case Ease::EaseBackInOut:
                return EaseBackInOut(t, b, c, d);
            case Ease::EaseBounceOut:
                return EaseBounceOut(t, b, c, d);
            case Ease::EaseBounceIn:
                return EaseBounceIn(t, b, c, d);
            case Ease::EaseBounceInOut:
                return EaseBounceInOut(t, b, c, d);
            case Ease::EaseElasticIn:
                return EaseElasticIn(t, b, c, d);
            case Ease::EaseElasticOut:
                return EaseElasticOut(t, b, c, d);
            case Ease::EaseElasticInOut:
                return EaseElasticInOut(t, b, c, d);
        }
        return EaseLinearNone(t, b, c, d);
    }

}  // namespace ClassicLauncher