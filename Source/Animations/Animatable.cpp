#include "Animatable.h"

#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{

    Animatable::Animatable(Window* window)
        : m_animationManager(window)
    {
        m_animationManager.SetStartCallback(std::bind(&Animatable::AnimationStarted, this, std::placeholders::_1));
        m_animationManager.SetRunningCallback(std::bind(&Animatable::AnimationUpdate, this, std::placeholders::_1));
        m_animationManager.SetFinishCallback(std::bind(&Animatable::AnimationFinished, this, std::placeholders::_1));
    }

    void Animatable::UpdateAnimation()
    {
        m_animationManager.Update();
    }

} // namespace ClassicLauncher
