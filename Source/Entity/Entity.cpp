#include "Entity.h"

#include <algorithm>

namespace ClassicLauncher
{


    Entity::Entity()
        : mToDelete(false)
        , mToDraw(true)
        , mScissorMode(false)
        , mVisible(true)
        , mZOrder(0)
        , mIdZOrder(0)
        , mId(0)
        , mChildEntities()
        , mNameId()
    {
    }

    void Entity::UpdateWorldTransform()
    {
        if (mParent)
        {
            m_worldTransform.position.x = mParent->m_worldTransform.position.x + m_transform.position.x    ;
            m_worldTransform.position.y = mParent->m_worldTransform.position.y + m_transform.position.y    ;
            m_worldTransform.offset.x = mParent->m_worldTransform.offset.x + m_transform.offset.x   ;
            m_worldTransform.offset.y = mParent->m_worldTransform.offset.y + m_transform.offset.y    ;
            m_worldTransform.scale.x = mParent->m_worldTransform.scale.x * m_transform.scale.x    ;
            m_worldTransform.scale.y = mParent->m_worldTransform.scale.y * m_transform.scale.y   ;
            m_worldTransform.origin.x = mParent->m_worldTransform.origin.x + m_transform.origin.x   ;
            m_worldTransform.origin.y = mParent->m_worldTransform.origin.y + m_transform.origin.y   ;
            m_worldTransform.rotation = mParent->m_worldTransform.rotation + m_transform.rotation  ;

            // Propaga a opacidade (alpha) do pai para o filho
            unsigned char parentAlpha = mParent->m_worldTransform.color.a;
            m_worldTransform.color.a = static_cast<unsigned char>((static_cast<int>(m_transform.color.a) * static_cast<int>(parentAlpha)) / 255);
            
            //m_worldAnimationTransform.position.x = mParent->m_worldAnimationTransform.position.x + m_animationTransform.position.x;
            //m_worldAnimationTransform.position.y = mParent->m_worldAnimationTransform.position.y + m_animationTransform.position.y;
            //m_worldAnimationTransform.offset.x = mParent->m_worldTransform.offset.x + m_transform.offset.x;
            //m_worldAnimationTransform.offset.y = mParent->m_worldTransform.offset.y + m_transform.offset.y;
            //m_worldAnimationTransform.scale.x = mParent->m_worldAnimationTransform.scale.x * m_animationTransform.scale.x;
            //m_worldAnimationTransform.scale.y = mParent->m_worldAnimationTransform.scale.y * m_animationTransform.scale.y;
            //m_worldAnimationTransform.rotation = mParent->m_worldAnimationTransform.rotation + m_animationTransform.rotation;
//
            //// Propaga a opacidade (alpha) do pai para o filho
            //unsigned char parentAlphaAni = mParent->m_worldTransform.color.a;
            //m_worldTransform.color.a = static_cast<unsigned char>((static_cast<int>(m_animationTransform.color.a) * static_cast<int>(parentAlphaAni)) / 255);

        }
        else
        {
            m_worldTransform = m_transform;
            m_worldAnimationTransform = m_animationTransform;
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
        mZOrder = zOrder;
    }

} // namespace ClassicLauncher
