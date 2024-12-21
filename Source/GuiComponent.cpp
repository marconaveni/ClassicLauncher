#include "GuiComponent.h"

namespace ClassicLauncher
{

    void GuiComponent::Update()
    {
        Entity::Update();
        for (auto& animation : mAnimations)
        {
            animation.second.Update(::GetFrameTime());
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