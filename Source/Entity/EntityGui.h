#ifndef ENTITY_GUI_H
#define ENTITY_GUI_H

#include <map>
#include <vector>

#include "Animations/Animation.h"
#include "Animations/AnimationManager.h"
#include "Entity/Entity.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{

    class Application;

    class EntityGui : public Entity
    {
    public:

        EntityGui();
        virtual void Update() override;
        virtual EntityType GetType() const override = 0;
        virtual void AnimationStarted(const std::string& name) {}
        virtual void AnimationUpdate(const std::string& name) {}
        virtual void AnimationFinished(const std::string& name) {}

        Animation& GetAnimation(const std::string& name) { return  m_animationManager.GetAnimation(name); }
        AnimationManager& GetAnimationManager() { return  m_animationManager; }

        // void StartAnimation(const std::string& name,
        //                     float durationAnimation,
        //                     const Transform& startAnimationTransform,
        //                     const Transform& finalAnimationTransform,
        //                     Ease typeAnimation = Ease::EaseLinearNone,
        //                     bool bForceReset = true);
        // void AddAnimationFrame(const std::string& name,
        //                        const float timeAnimation,
        //                        const std::vector<RectFloat>& spriteIndices);

        //virtual void UpdateTransformAnimation(const Animation& anim);
    private:
      
        // std::map<std::string, SpriteAnimator> mSpriteAnimations;
        // std::map<std::string, Animation> mAnimations;

        AnimationManager m_animationManager;
    };

} // namespace ClassicLauncher

#endif