#include "AnimationFrame.h"

void AnimationFrame::AddAnimation(const std::string& name, std::vector<Vector2> position)
{
	animations[name] = std::move(position);
}

std::vector<Vector2> AnimationFrame::GetAnimations(const std::string& name)
{
    std::map<std::string, std::vector<Vector2>>::iterator it;
    for (it = animations.begin(); it != animations.end(); ++it)
    {
        if (it->first == name)
        {
            return it->second;
        }
    }
    return {};
}

void AnimationFrame::UpdateAnimation()
{
    delay.SetStart();
    if (!delay.IsTimeElapsed())
    {
        delay.Reset();
        AnimationChangeFrame();
    }
}

void AnimationFrame::AnimationChangeFrame()
{
    frame++;
    if (static_cast<size_t>(frame) > GetAnimations(currentAnimation).size() - 1)
    {
        frame = 0;
    }
}

Vector2 AnimationFrame::GetCurrentFrame()
{
    const std::vector<Vector2> currentAni = GetAnimations(currentAnimation);
    if (currentAni.empty())
    {
        return Vector2{};
    }
    return currentAni[frame];
}
