#include "Entity.h"

#include <algorithm>

namespace ClassicLauncher
{


    Entity::Entity()
        : mToDelete(false)
        , mToDraw(true)
        , mScissorMode(false)
        , mVisible(true)
        , mChildEntities()
        , mNameId()
    {
    }

    void Entity::UpdateWorldTransform()
    {
        if (mParent)
        {
            const float parentX = mParent->m_worldTransform.position.x;
            const float parentY = mParent->m_worldTransform.position.y;
            

            m_worldTransform.position.x = parentX + (m_transform.position.x * mParent->m_worldTransform.scale.x);
            m_worldTransform.position.y = parentY + (m_transform.position.y * mParent->m_worldTransform.scale.y);

            // apply scale
            m_worldTransform.scale.x = mParent->m_worldTransform.scale.x * m_transform.scale.x;
            m_worldTransform.scale.y = mParent->m_worldTransform.scale.y * m_transform.scale.y;

            // apply offset
            m_worldTransform.offset.x = mParent->m_worldTransform.offset.x + m_transform.offset.x;
            m_worldTransform.offset.y = mParent->m_worldTransform.offset.y + m_transform.offset.y;

            m_worldTransform.rotation = mParent->m_worldTransform.rotation + m_transform.rotation;

            // Propaga a opacidade (alpha) do pai para o filho
            unsigned char parentAlpha = mParent->m_worldTransform.color.a;
            m_worldTransform.color.a = static_cast<unsigned char>((static_cast<int>(m_transform.color.a) * static_cast<int>(parentAlpha)) / 255);

        }
        else
        {
            m_worldTransform = m_transform;
        }

        for (auto& entity : mChildEntities)
        {
            entity->UpdateWorldTransform();
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
                                                return entity == childEntity; // Return true element
                                            }),
                             mChildEntities.end());
    }

    void Entity::RemoveAllChildren()
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

    std::vector<Entity*>& Entity::GetChildren()
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
        mScissorArea = RectFloat{x, y, width, height};
        for (auto& childEntity : mChildEntities)
        {
            childEntity->EnableScissorMode(mScissorArea.x, mScissorArea.y, mScissorArea.width, mScissorArea.height);
        }
    }

    void Entity::SetZOrder(int zOrder)
    {
        m_zOrder.id = zOrder;
    }

} // namespace ClassicLauncher
