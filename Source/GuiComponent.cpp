#include "GuiComponent.h"
#include "Graphics/SpriteAnimator.h"
#include "Application.h"

namespace ClassicLauncher
{
    GuiComponent::GuiComponent()
        : Entity()
    {
        mApplication = &Application::Get();
        CLASSIC_ASSERT(mApplication);
    }

    void GuiComponent::Update()
    {
        Entity::Update();
        for (auto& spriteAnimation : mSpriteAnimations)
        {
            spriteAnimation.second.Update(::GetFrameTime());
            Rectangle rec = spriteAnimation.second.GetCurrentSprite();
            mProperties.sourceX = rec.x;
            mProperties.sourceY = rec.y;
            mProperties.width = rec.width;
            mProperties.height = rec.height;
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
                mProperties.x = anim.mCurrentTransform.x;
                mProperties.y = anim.mCurrentTransform.y;
                mProperties.scaleX = anim.mCurrentTransform.scaleX;
                mProperties.scaleY = anim.mCurrentTransform.scaleY;
                mProperties.rotation = anim.mCurrentTransform.rotation;
                mProperties.color = anim.mCurrentTransform.color;
            }
            if (anim.mIsFinish)
            {
                AnimationFinished(name);
                anim.ResetAnimation();
                mProperties.x = anim.mCurrentTransform.x;
                mProperties.y = anim.mCurrentTransform.y;
                mProperties.scaleX = anim.mCurrentTransform.scaleX;
                mProperties.scaleY = anim.mCurrentTransform.scaleY;
                mProperties.rotation = anim.mCurrentTransform.rotation;
                mProperties.color = anim.mCurrentTransform.color;
            }
        }
    }

    void GuiComponent::StartAnimation(const std::string& name,
                                      float durationAnimation,
                                      const TransformProperties& startAnimationTransform,
                                      const TransformProperties& finalAnimationTransform,
                                      Ease typeAnimation,
                                      bool bForceReset)
    {
        Animation& anim = mAnimations[name];
        anim.StartAnimation(durationAnimation, startAnimationTransform, finalAnimationTransform, typeAnimation, bForceReset);
    }

    void GuiComponent::AddAnimationFrame(const std::string& name, const float timeAnimation, const std::vector<Rectangle>& spriteIndices)
    {
        if (mSpriteAnimations.find(name) != mSpriteAnimations.end())
        {
            mSpriteAnimations.erase(name);
        }
        mSpriteAnimations[name] = SpriteAnimator(timeAnimation, spriteIndices);
    }

}  // namespace ClassicLauncher