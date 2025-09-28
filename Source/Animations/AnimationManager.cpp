#include "AnimationManager.h"

#include "ClassicAssert.h"
#include "Window/RayWindow.h"
#include "Helper.h"


namespace ClassicLauncher
{
    AnimationManager::AnimationManager()
    {
    }

    void AnimationManager::Update()
    {

        CLASSIC_ASSERT(m_startCallback && m_runningCallback && m_finishCallback, "callbacks need to be registered");


        for (auto& spriteAnimation : m_spriteAnimations)
        {
            spriteAnimation.second.spriteAnimator.Update(RayWindow::GetFrameTime());
            Rectangle rec = spriteAnimation.second.spriteAnimator.GetCurrentSprite();
            spriteAnimation.second.transform->source.x = rec.x;
            spriteAnimation.second.transform->source.y = rec.y;
            spriteAnimation.second.transform->source.width = rec.width;
            spriteAnimation.second.transform->source.height = rec.height;
            spriteAnimation.second.transform->position.width = rec.width;
            spriteAnimation.second.transform->position.height = rec.height;
        }

        for (auto& animationTransform : m_animationsTransform)
        {
            const std::string& name = animationTransform.first;
            AnimationTransform& anim = animationTransform.second;
            Animation& animation = animationTransform.second.animation;


            if (animation.mIsStart)
            {
                animation.mIsStart = false;
                m_startCallback(name);
            }
            if (animation.mIsRunning)
            {
                animation.UpdateAnimation();
                UpdateTransformAnimation(anim);
                m_runningCallback(name);
                LOG(LOG_CLASSIC_TRACE, "%s", name.c_str());
            }
            if (animation.mIsFinish)
            {
                animation.ResetAnimation();
                UpdateTransformAnimation(anim);
                m_finishCallback(name);
                anim.transform = nullptr;
            }
        }
    }

    void AnimationManager::UpdateTransformAnimation(AnimationTransform& anim)
    {

        if (!anim.transform)
        {
            return;
        }

        *anim.transform = anim.animation.mCurrentTransform;
        
    }

    void AnimationManager::StartAnimation(const std::string& name,
                                          float durationAnimation,
                                          Transform* finalTransform,
                                          const Transform& targetTransform,
                                          Ease typeAnimation,
                                          bool bForceReset)
    {
        AnimationTransform& anim = m_animationsTransform[name];
        anim.transform = finalTransform;
        anim.animation.StartAnimation(durationAnimation, *finalTransform, targetTransform, typeAnimation, bForceReset);
    }

    void AnimationManager::AddAnimationFrame(const std::string& name,
                                             const float timeAnimation,
                                             Transform* transform,
                                             const std::vector<RectFloat>& spriteIndices)
    {
        if (m_spriteAnimations.find(name) != m_spriteAnimations.end())
        {
            m_spriteAnimations.erase(name);
        }
        m_spriteAnimations[name].spriteAnimator = SpriteAnimator(timeAnimation, spriteIndices);
        m_spriteAnimations[name].transform = transform;
    }

} // namespace ClassicLauncher
