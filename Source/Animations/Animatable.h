#ifndef ANIMATABLE_H
#define ANIMATABLE_H


#include "Animations/AnimationManager.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{


    class Animatable
    {
    public:

        Animatable();
        void UpdateAnimation();
        virtual void AnimationStarted(const std::string& name) {}
        virtual void AnimationUpdate(const std::string& name) {}
        virtual void AnimationFinished(const std::string& name) {}

        Animation& GetAnimation(const std::string& name) { return m_animationManager.GetAnimation(name); }
        AnimationManager& GetAnimationManager() { return m_animationManager; }

    private:

        AnimationManager m_animationManager{};
    };

} // namespace ClassicLauncher

#endif // ANIMATABLE_H