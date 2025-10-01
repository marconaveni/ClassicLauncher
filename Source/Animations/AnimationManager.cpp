#include "AnimationManager.h"

#include "ClassicAssert.h"
#include "Window/RayWindow.h"
#include "Entity/Entity.h"
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
            
            RectFloat rec = spriteAnimation.second.spriteAnimator.GetCurrentSprite();
            Entity* entity = spriteAnimation.second.entity;          
            
            entity->SetSource(rec);
            entity->SetSize(rec.width, rec.height);
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
                anim.entity = nullptr;
            }
        }
    }

    void AnimationManager::UpdateTransformAnimation(AnimationTransform& anim)
    {

        if (!anim.entity)
        {
            return;
        }

        const Transform& transform = anim.animation.mCurrentTransform;

        //*anim.transform = anim.animation.mCurrentTransform;
        anim.entity->SetPosition(transform.position.x, transform.position.y);
        anim.entity->SetOffset(transform.offset);
        anim.entity->SetScale(transform.scale);
        anim.entity->SetRotation(transform.rotation);
        anim.entity->SetColor(transform.color);
        
    }

    void AnimationManager::StartAnimation(const std::string& name,
                                          float durationAnimation,
                                          Entity* targetEntity,
                                          const Transform& targetTransform,
                                          Ease typeAnimation,
                                          bool bForceReset)
    {
        AnimationTransform& anim = m_animationsTransform[name];
        anim.entity = targetEntity;
        anim.animation.StartAnimation(durationAnimation, targetEntity->GetTransform(), targetTransform, typeAnimation, bForceReset);
    }

    void AnimationManager::AddAnimationFrame(const std::string& name,
                                             const float timeAnimation,
                                             Entity* targetEntity,
                                             const std::vector<RectFloat>& spriteIndices)
    {
        if (m_spriteAnimations.find(name) != m_spriteAnimations.end())
        {
            m_spriteAnimations.erase(name);
        }
        m_spriteAnimations[name].spriteAnimator = SpriteAnimator(timeAnimation, spriteIndices);
        m_spriteAnimations[name].entity = targetEntity;
    }

} // namespace ClassicLauncher
