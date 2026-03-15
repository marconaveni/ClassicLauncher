#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Animations/Animation.h"
#include "Data/Transform.h"
#include "Graphics/SpriteAnimator.h"


namespace ClassicLauncher
{

    class Entity;

    class AnimationManager
    {
    public:


        AnimationManager();
        ~AnimationManager() = default;
        void Update();
        Animation& GetAnimation(const std::string& name) { return m_animationsTransform[name].animation; }

        void StartAnimation(const std::string& name,
                            float durationAnimation,
                            Entity* targetEntity,
                            const Transform& targetTransform,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool forceReset = true);

        void AddAnimationFrame(const std::string& name, const float timeAnimation, Entity* targetEntity, const std::vector<RectFloat>& spriteIndices);

        void SetStartCallback(std::function<void(const std::string&)> callback) { m_startCallback = std::move(callback); }
        void SetRunningCallback(std::function<void(const std::string&)> callback) { m_runningCallback = std::move(callback); }
        void SetFinishCallback(std::function<void(const std::string&)> callback) { m_finishCallback = std::move(callback); }

    private:

        struct AnimationTransform
        {
            Animation animation{};
            Entity* entity = nullptr;
        };

        struct AnimationSpriteSource
        {
            SpriteAnimator spriteAnimator{};
            Entity* entity = nullptr;
        };

        void UpdateTransformAnimation(AnimationTransform& anim);

        std::function<void(const std::string&)> m_startCallback;
        std::function<void(const std::string&)> m_runningCallback;
        std::function<void(const std::string&)> m_finishCallback;


        std::map<std::string, AnimationSpriteSource> m_spriteAnimations;
        std::map<std::string, AnimationTransform> m_animationsTransform;
    };
} // namespace ClassicLauncher

#endif // ANIMATION_MANAGER_H