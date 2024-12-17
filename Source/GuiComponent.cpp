#include "GuiComponent.h"
#include "Utils/Core.h"
#include "raylib.h"

namespace ClassicLauncher
{

    void GuiComponent::Update()
    {
        //relativeX;
        //relativeY = 0;
        Entity::Update();
    }

    void GuiComponent::UpdatePosition()
    {

        for (auto& entity : childEntities)
        {
            entity->rootX = this->x + rootX;
            entity->rootY = this->y + rootY;
        }
    }

    void GuiComponent::AddChild(GuiComponent* childEntity)
    {
        if(childEntity->parent != this) 
        {
            childEntity->parent = this;
            childEntities.emplace_back(childEntity);
        }
    }

    void GuiComponent::RemoveChild()
    {
    }

    std::vector<GuiComponent*> GuiComponent::GetChilds()
    {
        return childEntities;
    }

    void GuiComponent::SelfDelete()
    {
        Entity::SelfDelete();
        for (auto& entity : childEntities)
        {
            entity->SelfDelete();
        }
        
    }

    GuiComponent* GuiComponent::GetRootEntity()
    {
        GuiComponent* rootParent = parent;
        if(rootParent != nullptr) 
        {
            rootParent = rootParent->GetRootEntity();
        }
        return (rootParent != nullptr) ? rootParent : this;
    }
}