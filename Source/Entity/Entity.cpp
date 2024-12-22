#include "Entity.h"

namespace ClassicLauncher
{

    void Entity::UpdatePosition()
    {
        for (auto& entity : mChildEntities)
        {
            entity->mProperties.rootX = mProperties.x + mProperties.rootX;
            entity->mProperties.rootY = mProperties.y + mProperties.rootY;
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
            mScissorArea.SetPosition(Math::Max(x, mScissorArea.x), Math::Max(y, mScissorArea.y));
            mScissorArea.SetSize(Math::Min(width, mScissorArea.width), Math::Min(height, mScissorArea.height));
        }
        else
        {
            mScissorArea.Set(x, y, width, height);
        }

        bScissorMode = true;
        for (auto& childEntity : mChildEntities)
        {
            childEntity->EnableScissorMode(mScissorArea.x, mScissorArea.y, mScissorArea.width, mScissorArea.height);
        }
    }

    void Entity::SetZOrder(int zOrder)
    {
        mZOrder = zOrder;
    }

}  // namespace ClassicLauncher
