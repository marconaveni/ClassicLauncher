#include "Entity.h"

namespace ClassicLauncher
{
    Entity::Entity()
        : mToDelete(false), mToDraw(true), mScissorMode(false), mVisible(true), mZOrder(0), mIdZOrder(0), mId(0), mChildEntities(), mNameId()
    {
    }

    void Entity::UpdatePosition()
    {
        for (auto& entity : mChildEntities)
        {
            // entity->mTransform.rootX = mTransform.x + mTransform.rootX;
            // entity->mTransform.rootY = mTransform.y + mTransform.rootY;
            // entity->mTransform.rootScaleX = mTransform.scaleX * mTransform.rootScaleX;
            // entity->mTransform.rootScaleY = mTransform.scaleY * mTransform.rootScaleY;
            // entity->mTransform.color.a = mTransform.color.a <= entity->mTransform.color.a ? mTransform.color.a : entity->mTransform.color.a;
            mTransform.UpdateTransform(entity->mTransform);
            entity->UpdatePosition();
        }
    }

    void Entity::SelfDelete()
    {
        mToDelete = true;
        for (auto& entity : mChildEntities)
        {
            entity->SelfDelete();
            Entity* e = GetRootEntity();
            if (e != this)
            {
                e->RemoveChild(this);
            }
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

    void Entity::RemoveChild(Entity* childEntity)
    {
        mChildEntities.erase(std::remove_if(mChildEntities.begin(),
                                            mChildEntities.end(),
                                            [childEntity](const Entity* entity)
                                            {
                                                return entity == childEntity;  // Return true element
                                            }),
                             mChildEntities.end());
    }

    void Entity::RemoveAllChilds()
    {
        mChildEntities.clear();
    }

    void Entity::RemoveRootChild()
    {
        Entity* entity = GetRootEntity();
        if (entity != this)
        {
            entity->RemoveChild(this);
        }
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
        mScissorMode = true;
        mScissorArea.Set(x, y, width, height);
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
