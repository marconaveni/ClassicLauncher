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

    class AnimationManager
    {
    public:




        AnimationManager(Transform& transform);
        ~AnimationManager() = default;
        void Update();
        Animation& GetAnimation(const std::string& name) { return m_animations[name]; }
        void UpdateTransformAnimation(const Animation& anim);
        void StartAnimation(const std::string& name,
                            float durationAnimation,
                            const Transform& startAnimationTransform,
                            const Transform& finalAnimationTransform,
                            Ease typeAnimation = Ease::EaseLinearNone,
                            bool bForceReset = true);
        void AddAnimationFrame(const std::string& name,
                               const float timeAnimation,
                               const std::vector<RectFloat>& spriteIndices);

        void SetStartCallback(std::function<void(const std::string&)> callback)
        {
            m_startCallback = std::move(callback);
        }
        void SetRunningCallback(std::function<void(const std::string&)> callback)
        {
            m_runningCallback = std::move(callback);
        }
        void SetFinishCallback(std::function<void(const std::string&)> callback)
        {
            m_finishCallback = std::move(callback);
        }

    private:

        std::function<void(const std::string&)> m_startCallback;
        std::function<void(const std::string&)> m_runningCallback;
        std::function<void(const std::string&)> m_finishCallback;

        Transform& m_transform;
        std::map<std::string, SpriteAnimator> m_spriteAnimations;
        std::map<std::string, Animation> m_animations;
    };
} // namespace ClassicLauncher

#endif // ANIMATION_MANAGER_H