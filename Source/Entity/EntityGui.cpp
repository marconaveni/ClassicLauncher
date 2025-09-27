#include "EntityGui.h"

#include "Graphics/SpriteAnimator.h"
#include "Window/RayWindow.h"

namespace ClassicLauncher
{

    EntityGui::EntityGui()
     : m_animationManager(m_transform)
    {
        m_animationManager.SetStartCallback(std::bind(&EntityGui::AnimationStarted, this, std::placeholders::_1));
        m_animationManager.SetRunningCallback(std::bind(&EntityGui::AnimationUpdate, this, std::placeholders::_1));
        m_animationManager.SetFinishCallback(std::bind(&EntityGui::AnimationFinished, this, std::placeholders::_1));
    }

    void EntityGui::Update()
    {
        Entity::Update();
        m_animationManager.Update();
    }

} // namespace ClassicLauncher
