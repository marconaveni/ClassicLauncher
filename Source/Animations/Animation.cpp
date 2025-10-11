#include "Animation.h"

#include "Window/RayWindow.h"
#include "reasings/reasings.h"

namespace ClassicLauncher
{


    void Animation::StartAnimation(const float durationAnimation,
                                   const Transform& startAnimation,
                                   const Transform& targetAnimation,
                                   const Ease typeAnimation,
                                   const bool forceReset)
    {
        m_isRunning = true;
        m_isStart = true;
        m_isFinish = false;
        m_currentTime = 0.0f;
        m_duration = durationAnimation;
        m_startTransform = startAnimation;
        m_currentTransform = startAnimation;
        m_finalTransform = targetAnimation;
        m_isReset = forceReset;
        m_typeEase = typeAnimation;
    }

    void Animation::UpdateAnimation()
    {
       // mIsFinish = false;
        if (m_isRunning)
        {
            if (m_currentTime <= m_duration)
            {
                m_currentTransform.position.x = GetAnimation(m_currentTime,
                                                            m_startTransform.position.x,
                                                            m_finalTransform.position.x - m_startTransform.position.x,
                                                            m_duration);
                m_currentTransform.position.y = GetAnimation(m_currentTime,
                                                            m_startTransform.position.y,
                                                            m_finalTransform.position.y - m_startTransform.position.y,
                                                            m_duration);
                m_currentTransform.offset.x = GetAnimation(m_currentTime,
                                                            m_startTransform.offset.x,
                                                            m_finalTransform.offset.x - m_startTransform.offset.x,
                                                            m_duration);
                m_currentTransform.offset.y = GetAnimation(m_currentTime,
                                                            m_startTransform.offset.y,
                                                            m_finalTransform.offset.y - m_startTransform.offset.y,
                                                            m_duration);
                m_currentTransform.scale.x = GetAnimation(m_currentTime,
                                                         m_startTransform.scale.x,
                                                         m_finalTransform.scale.x - m_startTransform.scale.x,
                                                         m_duration);
                m_currentTransform.scale.y = GetAnimation(m_currentTime,
                                                         m_startTransform.scale.y,
                                                         m_finalTransform.scale.y - m_startTransform.scale.y,
                                                         m_duration);
                m_currentTransform.rotation = GetAnimation(m_currentTime,
                                                          m_startTransform.rotation,
                                                          m_finalTransform.rotation - m_startTransform.rotation,
                                                          m_duration);
                m_currentTransform.color.r = GetAnimation(m_currentTime,
                                                         m_startTransform.color.r,
                                                         m_finalTransform.color.r - m_startTransform.color.r,
                                                         m_duration);
                m_currentTransform.color.g = GetAnimation(m_currentTime,
                                                         m_startTransform.color.g,
                                                         m_finalTransform.color.g - m_startTransform.color.g,
                                                         m_duration);
                m_currentTransform.color.b = GetAnimation(m_currentTime,
                                                         m_startTransform.color.b,
                                                         m_finalTransform.color.b - m_startTransform.color.b,
                                                         m_duration);
                m_currentTransform.color.a = GetAnimation(m_currentTime,
                                                         m_startTransform.color.a,
                                                         m_finalTransform.color.a - m_startTransform.color.a,
                                                         m_duration);

                m_currentTime += RayWindow::GetFrameTime();
                return;
            }
            m_isRunning = false;
            FinishAnimation();
        }
    }

    bool Animation::FinishAnimation()
    {
        m_isFinish = true;
        return ResetAnimation();
    }

    bool Animation::ResetAnimation()
    {
        m_currentTime = 0;
        m_currentTransform = (m_isReset) ? m_startTransform : m_finalTransform;
        return m_isReset;
    }

    bool Animation::GetAnimationIsRun() const
    {
        return m_isRunning;
    }

    bool Animation::GetAnimationFinish() const
    {
        return m_isFinish;
    }

    float Animation::GetAnimation(const float t, const float b, const float c, const float d) const
    {
        switch (m_typeEase)
        {
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

} // namespace ClassicLauncher