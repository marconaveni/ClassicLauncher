#ifndef ENTITY_GUI_H
#define ENTITY_GUI_H

#include <map>
#include <memory>
#include <vector>
#include "Animations/Animation.h"
#include "Graphics/SpriteAnimator.h"
#include "Entity/Entity.h"

namespace ClassicLauncher
{

    class Application;

    class EntityGui : public Entity
    {
        std::map<std::string, SpriteAnimator> mSpriteAnimations;
        std::map<std::string, Animation> mAnimations;
        Application* mApplication;
        void UpdateTransform(const Animation& anim);

    public:

        EntityGui();
        virtual void Update() override;
        virtual EntityType GetType() const override = 0;
        virtual void AnimationStarted(std::string name) {}
        virtual void AnimationUpdate(std::string name) {}
        virtual void AnimationFinished(std::string name) {}

        Application* GetApplication() { return mApplication; }
        Animation& GetAnimation(const std::string& name) { return mAnimations[name]; }

        void StartAnimation(const std::string& name,
                            float durationAnimation,
                            const Transformation& startAnimationTransform,
                            const Transformation& finalAnimationTransform,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool bForceReset = true);
        void AddAnimationFrame(const std::string& name, const float timeAnimation, const std::vector<RectFloat>& spriteIndices);
    };

}  // namespace ClassicLauncher

#endif