#include "AnimationManager.h"

#include "ClassicAssert.h"
#include "Entity/Entity.h"
#include "Helper.h"
#include "Window/Window.h"


namespace ClassicLauncher
{

    AnimationManager::AnimationManager(Window* window)
        : m_windowRef(window)
    {
    }

    void AnimationManager::Update()
    {

        CLASSIC_ASSERT(m_startCallback && m_runningCallback && m_finishCallback, "callbacks need to be registered");


        for (auto& spriteAnimation : m_spriteAnimations)
        {
            spriteAnimation.second.spriteAnimator.Update(m_windowRef->GetFrameTime());

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


            if (animation.m_isStart)
            {
                animation.m_isStart = false;
                m_startCallback(name);
            }
            if (animation.m_isRunning)
            {
                animation.UpdateAnimation();
                UpdateTransformAnimation(anim);
                m_runningCallback(name);
                LOG(LogClassicTrace, "%s", name.c_str());
            }
            if (animation.m_isFinish)
            {
                animation.ResetAnimation();
                UpdateTransformAnimation(anim);
                animation.m_isFinish = false;
                m_finishCallback(name);
                anim.entity = nullptr;
            }
        }
    }

    void AnimationManager::UpdateTransformAnimation(AnimationTransform& animationTransform)
    {

        if (!animationTransform.entity)
        {
            return;
        }

        const Transform& transform = animationTransform.animation.m_currentTransform;

        animationTransform.entity->SetPosition(transform.position.x, transform.position.y);
        animationTransform.entity->SetOffset(transform.offset);
        animationTransform.entity->SetScale(transform.scale);
        animationTransform.entity->SetRotation(transform.rotation);
        animationTransform.entity->SetColor(transform.color);
    }

    void AnimationManager::StartAnimation(const std::string& name,
                                          float durationAnimation,
                                          Entity* targetEntity,
                                          const Transform& targetTransform,
                                          Ease typeAnimation,
                                          bool forceReset)
    {
        AnimationTransform& animationTransform = m_animationsTransform[name];
        animationTransform.entity = targetEntity;
        animationTransform.animation.SetWindow(m_windowRef);
        animationTransform.animation.StartAnimation(durationAnimation, targetEntity->GetTransform(), targetTransform, typeAnimation, forceReset);
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
