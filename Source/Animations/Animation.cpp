#include "Animation.h"
#include "reasings/reasings.h"

namespace ClassicLauncher
{

    Animation::Animation()
        : mCurrentTime(0), mDuration(0), bStart(false), bRunning(false), bFinish(false), bReset(false), bRelative(false), mType(Ease::EaseLinearNone) {};

    void Animation::StartAnimation(const float durationAnimation,
                                   const TransformProperties& startAnimation,
                                   const TransformProperties& finalAnimation,
                                   const Ease typeAnimation,
                                   const bool bForceReset)
    {
        bRunning = true; 
        bStart = true;
        bFinish = false;
        mCurrentTime = 0.0f;
        mDuration = durationAnimation / GetFrameTime(); 
        mStartTransform = startAnimation;
        mCurrentTransform = startAnimation;
        mFinalTransform = finalAnimation;
        bReset = bForceReset;
        mType = typeAnimation;
    }

    void Animation::UpdateAnimation()
    {
        bFinish = false;
        if (bRunning)
        {
            if (mCurrentTime <= mDuration)
            {
                mCurrentTransform.x = GetAnimation(mCurrentTime, mStartTransform.x, mFinalTransform.x - mStartTransform.x, mDuration);
                mCurrentTransform.y = GetAnimation(mCurrentTime, mStartTransform.y, mFinalTransform.y - mStartTransform.y, mDuration);
                mCurrentTransform.scale = GetAnimation(mCurrentTime, mStartTransform.scale, mFinalTransform.scale - mStartTransform.scale, mDuration);
                mCurrentTransform.rotation = GetAnimation(mCurrentTime, mStartTransform.rotation, mFinalTransform.rotation - mStartTransform.rotation, mDuration);

                // const float r = GetAnimation(static_cast<float>(currentTime),
                //                              static_cast<float>(startPosition.color.GetColorRed()),
                //                              static_cast<float>(finalPosition.color.GetColorRed() - startPosition.color.GetColorRed()),
                //                              static_cast<float>(duration));
                // const float g = GetAnimation(static_cast<float>(currentTime),
                //                              static_cast<float>(startPosition.color.GetColorGreen()),
                //                              static_cast<float>(finalPosition.color.GetColorGreen() - startPosition.color.GetColorGreen()),
                //                              static_cast<float>(duration));
                // const float b = GetAnimation(static_cast<float>(currentTime),
                //                              static_cast<float>(startPosition.color.GetColorBlue()),
                //                              static_cast<float>(finalPosition.color.GetColorBlue() - startPosition.color.GetColorBlue()),
                //                              static_cast<float>(duration));
                // const float a = GetAnimation(static_cast<float>(currentTime),
                //                              static_cast<float>(startPosition.color.GetColorAlpha()),
                //                              static_cast<float>(finalPosition.color.GetColorAlpha() - startPosition.color.GetColorAlpha()),
                //                              static_cast<float>(duration));

                // currentPosition.color.SetColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a));

                mCurrentTime++;
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
        bRunning = false;
        mCurrentTransform = (bReset) ? mStartTransform : mFinalTransform;
        mCurrentTime = 0;
    }

    bool Animation::GetAnimationIsRun() const
    {
        return bRunning;
    }

    bool Animation::GetAnimationFinish() const
    {
        return bFinish;
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