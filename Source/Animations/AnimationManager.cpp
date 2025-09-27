#include "AnimationManager.h"

#include "Window/RayWindow.h"


namespace ClassicLauncher
{
    AnimationManager::AnimationManager(Transform& transform)
        : m_transform(transform)
    {
    }

    void AnimationManager::Update()
    {
        for (auto& spriteAnimation : m_spriteAnimations)
        {
            spriteAnimation.second.Update(RayWindow::GetFrameTime());
            Rectangle rec = spriteAnimation.second.GetCurrentSprite();
            m_transform.source.x = rec.x;
            m_transform.source.y = rec.y;
            m_transform.source.width = rec.width;
            m_transform.source.height = rec.height;
            m_transform.position.width = rec.width;
            m_transform.position.height = rec.height;
        }

        for (auto& animation : m_animations)
        {
            const std::string& name = animation.first;
            Animation& anim = animation.second;

            if (anim.mIsStart)
            {
                m_startCallback(name);
                anim.mIsStart = false;
            }
            anim.UpdateAnimation();
            if (anim.mIsRunning)
            {
                m_runningCallback(name);
                UpdateTransformAnimation(anim);
            }
            if (anim.mIsFinish)
            {
                m_finishCallback(name);
                anim.ResetAnimation();
                UpdateTransformAnimation(anim);
            }
        }
    }

    void AnimationManager::UpdateTransformAnimation(const Animation& anim)
    {
        m_transform.position.x = anim.mCurrentTransform.position.x;
        m_transform.position.y = anim.mCurrentTransform.position.y;
        m_transform.offset.x = anim.mCurrentTransform.offset.x;
        m_transform.offset.y = anim.mCurrentTransform.offset.y;
        m_transform.scale.x = anim.mCurrentTransform.scale.x;
        m_transform.scale.y = anim.mCurrentTransform.scale.y;
        m_transform.rotation = anim.mCurrentTransform.rotation;
        m_transform.color = anim.mCurrentTransform.color;
    }

    void AnimationManager::StartAnimation(const std::string& name,
                                          float durationAnimation,
                                          const Transform& startAnimationTransform,
                                          const Transform& finalAnimationTransform,
                                          Ease typeAnimation,
                                          bool bForceReset)
    {
        Animation& anim = m_animations[name];
        anim.StartAnimation(durationAnimation,
                            startAnimationTransform,
                            finalAnimationTransform,
                            typeAnimation,
                            bForceReset);
    }

    void AnimationManager::AddAnimationFrame(const std::string& name,
                                             const float timeAnimation,
                                             const std::vector<RectFloat>& spriteIndices)
    {
        if (m_spriteAnimations.find(name) != m_spriteAnimations.end())
        {
            m_spriteAnimations.erase(name);
        }
        m_spriteAnimations[name] = SpriteAnimator(timeAnimation, spriteIndices);
    }

} // namespace ClassicLauncher
