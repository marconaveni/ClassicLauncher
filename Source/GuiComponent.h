#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <map>
#include <memory>
#include <vector>
#include "Animations/Animation.h"
#include "Entity/Entity.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{
    class GuiComponent : public Entity
    {
        std::map<std::string, SpriteAnimator> mSpriteAnimations;
        std::map<std::string, Animation> mAnimations;

    public:

        GuiComponent() = default;
        virtual ~GuiComponent() override;
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
        virtual void Update() override;

        virtual void AnimationStarted(std::string name) {}
        virtual void AnimationUpdate(std::string name) {}
        virtual void AnimationFinished(std::string name) {}

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

#endif  // GUI_COMPONENT_H