#include "EntityGui.h"


#include "Graphics/SpriteAnimator.h"
#include "Window/RayWindow.h"

namespace ClassicLauncher
{

    EntityGui::EntityGui()
    {
    }

    void EntityGui::Update()
    {
        Entity::Update();
        for (auto& spriteAnimation : mSpriteAnimations)
        {
            spriteAnimation.second.Update(RayWindow::GetFrameTime());
            Rectangle rec = spriteAnimation.second.GetCurrentSprite();
            m_transform.source.x = rec.x;
            m_transform.source.y = rec.y;
            m_transform.position.width = rec.width;
            m_transform.position.height = rec.height;
        }

        for (auto& animation : mAnimations)
        {
            const std::string& name = animation.first;
            Animation& anim = animation.second;

            if (anim.mIsStart)
            {
                AnimationStarted(name);
                anim.mIsStart = false;
            }
            anim.UpdateAnimation();
            if (anim.mIsRunning)
            {
                AnimationUpdate(name);
                UpdateTransform(anim);
            }
            if (anim.mIsFinish)
            {
                AnimationFinished(name);
                anim.ResetAnimation();
                UpdateTransform(anim);
            }
        }
    }

    void EntityGui::UpdateTransform(const Animation& anim)
    {
        m_transform.position.x = anim.mCurrentTransform.position.x;
        m_transform.position.y = anim.mCurrentTransform.position.y;
        m_transform.scale.x = anim.mCurrentTransform.scale.x;
        m_transform.scale.y = anim.mCurrentTransform.scale.y;
        m_transform.rotation = anim.mCurrentTransform.rotation;
        m_transform.color = anim.mCurrentTransform.color;
    }

    void EntityGui::StartAnimation(const std::string& name,
                                   float durationAnimation,
                                   const Transform& startAnimationTransform,
                                   const Transform& finalAnimationTransform,
                                   Ease typeAnimation,
                                   bool bForceReset)
    {
        Animation& anim = mAnimations[name];
        anim.StartAnimation(durationAnimation,
                            startAnimationTransform,
                            finalAnimationTransform,
                            typeAnimation,
                            bForceReset);
    }

    void EntityGui::AddAnimationFrame(const std::string& name,
                                      const float timeAnimation,
                                      const std::vector<RectFloat>& spriteIndices)
    {
        if (mSpriteAnimations.find(name) != mSpriteAnimations.end())
        {
            mSpriteAnimations.erase(name);
        }
        mSpriteAnimations[name] = SpriteAnimator(timeAnimation, spriteIndices);
    }

} // namespace ClassicLauncher
