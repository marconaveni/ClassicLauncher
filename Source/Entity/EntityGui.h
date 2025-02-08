#ifndef ENTITY_GUI_H
#define ENTITY_GUI_H

#include <map>
#include <memory>
#include <vector>
#include "Animations/Animation.h"
#include "Entity/Entity.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{

    class EntityGui : public Entity
    {
        std::map<std::string, SpriteAnimator> mSpriteAnimations;
        std::map<std::string, Animation> mAnimations;
        Application* mApplication;

    public:

        EntityGui();
        virtual ~EntityGui() override = default;
        virtual void Update() override;
        virtual EntityType GetType() const override = 0;
        virtual void AnimationStarted(std::string name) {}
        virtual void AnimationUpdate(std::string name) {}
        virtual void AnimationFinished(std::string name) {}

        Application* GetApplication() { return mApplication; }
        Animation& GetAnimation(const std::string& name) { return mAnimations[name]; }

        void StartAnimation(const std::string& name,
                            float durationAnimation,
                            const TransformProperties& startAnimationTransform,
                            const TransformProperties& finalAnimationTransform,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool bForceReset = true);
        void AddAnimationFrame(const std::string& name, const float timeAnimation, const std::vector<Rectangle>& spriteIndices);
    };

}  // namespace ClassicLauncher

#endif