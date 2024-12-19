#include "GuiComponent.h"


namespace ClassicLauncher
{

    void GuiComponent::Update()
    {
        // relativeX;
        // relativeY = 0;
        Entity::Update();
    }

    void GuiComponent::UpdatePosition()
    {

        for (auto& entity : mChildEntities)
        {
            entity->rootX = this->x + rootX;
            entity->rootY = this->y + rootY;
        }
    }

    void GuiComponent::AddChild(GuiComponent* childEntity)
    {
        if (childEntity->mParent != this)
        {
            childEntity->mParent = this;
            mChildEntities.emplace_back(childEntity);
        }
    }

    void GuiComponent::RemoveChild() {}

    std::vector<GuiComponent*> GuiComponent::GetChilds() { return mChildEntities; }

    void GuiComponent::SelfDelete()
    {
        Entity::SelfDelete();
        for (auto& entity : mChildEntities)
        {
            entity->SelfDelete();
        }
    }

    GuiComponent* GuiComponent::GetRootEntity()
    {
        GuiComponent* rootParent = mParent;
        if (rootParent != nullptr)
        {
            rootParent = rootParent->GetRootEntity();
        }
        return (rootParent != nullptr) ? rootParent : this;
    }
}  // namespace ClassicLauncher