#include "EntityGui.h"
#include "Graphics/SpriteAnimator.h"
#include "Application.h"

namespace ClassicLauncher
{
    
    EntityGui::EntityGui()
    {
        mApplication = &Application::Get();
        CLASSIC_ASSERT(mApplication);
    }
    
    void EntityGui::Update()
    {
        Entity::Update();
        for (auto& spriteAnimation : mSpriteAnimations)
        {
            spriteAnimation.second.Update(::GetFrameTime());
            Rectangle rec = spriteAnimation.second.GetCurrentSprite();
            mTransform.sourceX = rec.x;
            mTransform.sourceY = rec.y;
            mTransform.width = rec.width;
            mTransform.height = rec.height;
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
        mTransform.x = anim.mCurrentTransform.x;
        mTransform.y = anim.mCurrentTransform.y;
        mTransform.scaleX = anim.mCurrentTransform.scaleX;
        mTransform.scaleY = anim.mCurrentTransform.scaleY;
        mTransform.rotation = anim.mCurrentTransform.rotation;
        mTransform.color = anim.mCurrentTransform.color;
    }

    void EntityGui::StartAnimation(const std::string& name,
                                   float durationAnimation,
                                   const Transform& startAnimationTransform,
                                   const Transform& finalAnimationTransform,
                                   Ease typeAnimation,
                                   bool bForceReset)
    {
        Animation& anim = mAnimations[name];
        anim.StartAnimation(durationAnimation, startAnimationTransform, finalAnimationTransform, typeAnimation, bForceReset);
    }

    void EntityGui::AddAnimationFrame(const std::string& name, const float timeAnimation, const std::vector<Rectangle>& spriteIndices)
    {
        if (mSpriteAnimations.find(name) != mSpriteAnimations.end())
        {
            mSpriteAnimations.erase(name);
        }
        mSpriteAnimations[name] = SpriteAnimator(timeAnimation, spriteIndices);
    }

} // namespace ClassicLauncher
