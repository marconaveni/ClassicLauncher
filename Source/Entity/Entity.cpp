#include "Entity.h"

namespace ClassicLauncher
{

    void Entity::UpdatePosition()
    {
        for (auto& entity : mChildEntities)
        {
            entity->rootX = this->x + rootX;
            entity->rootY = this->y + rootY;
        }
    }

    void Entity::SelfDelete()
    {
        bToDelete = true;
        for (auto& entity : mChildEntities)
        {
            entity->SelfDelete();
        }
    }

    void Entity::AddChild(Entity* childEntity)
    {
        if (childEntity->mParent != this)
        {
            childEntity->mParent = this;
            mChildEntities.emplace_back(childEntity);
        }
    }

    void Entity::RemoveChild()
    {
    }

    std::vector<Entity*>& Entity::GetChilds()
    {
        return mChildEntities;
    }

    Entity* Entity::GetRootEntity()
    {
        Entity* rootParent = mParent;
        if (rootParent != nullptr)
        {
            rootParent = rootParent->GetRootEntity();
        }
        return (rootParent != nullptr) ? rootParent : this;
    }

    void Entity::EnableScissorMode(float x, float y, float width, float height)
    {
        if (bScissorMode)
        {
            scissorArea.SetPosition(Math::Max(x, scissorArea.x), Math::Max(y, scissorArea.y));
            scissorArea.SetSize(Math::Min(width, scissorArea.width), Math::Min(height, scissorArea.height));
        }
        else
        {
            scissorArea.Set(x, y, width, height);
        }

        bScissorMode = true;
        for (auto& childEntity : mChildEntities)
        {
            childEntity->EnableScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
        }
    }

    void Entity::SetZOrder(int zOrder)
    {
        mZOrder = zOrder;
    }

}  // namespace ClassicLauncher
