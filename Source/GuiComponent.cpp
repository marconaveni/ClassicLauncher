#include "GuiComponent.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{

    void GuiComponent::Update()
    {
        Entity::Update();
        for (auto& animation : mAnimations)
        {
            animation.second.Update(::GetFrameTime());
            Rectangle rec = animation.second.GetCurrentSprite();
            mProperties.sourceX = rec.x;
            mProperties.sourceY = rec.y;
            mProperties.width = rec.width;
            mProperties.height = rec.height;
        }        
    }

    void GuiComponent::AddAnimationFrame(const std::string& name, const float timeAnimation, const std::vector<Rectangle>& spriteIndices)
    {
        if (mAnimations.find(name) != mAnimations.end())
        {
            mAnimations.erase(name);
        }
        mAnimations[name] = SpriteAnimator(timeAnimation, spriteIndices);
    }

}  // namespace ClassicLauncher