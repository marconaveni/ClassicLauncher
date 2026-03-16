#ifndef ANIMATION_H
#define ANIMATION_H

#include "Data/Transform.h"

namespace ClassicLauncher
{
    class Window;

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

        Animation() = default;

        float m_currentTime{0.0f};
        float m_duration{0.0f};

        bool m_isStart{false};
        bool m_isRunning{false};
        bool m_isFinish{false};
        bool m_isReset{false};

        Ease m_typeEase{Ease::EaseLinearNone};

        Transform m_startTransform{};
        Transform m_currentTransform{};
        Transform m_finalTransform{};


        void StartAnimation(float durationAnimation,
                            const Transform& startAnimation,
                            const Transform& targetAnimation,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool forceReset = true);
        void UpdateAnimation();
        bool FinishAnimation();
        bool ResetAnimation();
        bool GetAnimationIsRun() const;
        bool GetAnimationFinish() const;
        void SetWindow(Window* window) { m_windowRef = window; }

    private:

        float GetAnimation(float t, float b, float c, float d) const;            
        Window* m_windowRef{nullptr};
    };

} // namespace ClassicLauncher

#endif // ANIMATION_H
