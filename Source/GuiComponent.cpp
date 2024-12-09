#include "GuiComponent.h"

namespace ClassicLauncher
{

    void GuiComponent::Update()
    {
        for (auto& entity : entityChildrens)
        {
            entity->relativeX = x;
            entity->relativeY = y;
        }
    }

    void GuiComponent::AddChild(GuiComponent* entityChildren)
    {
        entityChildrens.emplace_back(entityChildren);
    }

    void GuiComponent::RemoveChild()
    {
    }

    void GuiComponent::SelfDelete()
    {
        Entity::SelfDelete();
        for (auto& entity : entityChildrens)
        {
            entity->SelfDelete();
        }
        
    }

}