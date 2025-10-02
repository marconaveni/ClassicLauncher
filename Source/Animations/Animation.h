#ifndef ANIMATION_H
#define ANIMATION_H

#include "Data/Transform.h"

namespace ClassicLauncher
{

    enum class Ease : std::uint8_t
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

        float m_currentTime;
        float m_duration;

        bool m_isStart;
        bool m_isRunning;
        bool m_isFinish;
        bool m_isReset;

        Ease m_typeEase;

        Transform m_startTransform;
        Transform m_currentTransform;
        Transform m_finalTransform;

        Animation();

        void StartAnimation(float durationAnimation,
                            const Transform& startAnimation,
                            const Transform& targetAnimation,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool bForceReset = true);
        void UpdateAnimation();
        bool FinishAnimation();
        bool ResetAnimation();
        bool GetAnimationIsRun() const;
        bool GetAnimationFinish() const;

    protected:

        float GetAnimation(float t, float b, float c, float d) const;
    };

} // namespace ClassicLauncher

#endif // ANIMATION_H
