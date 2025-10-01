#include "Entity.h"

#include <algorithm>

namespace ClassicLauncher
{


    Entity::Entity()
        : mToDelete(false), mToDraw(true), mScissorMode(false), mVisible(true), mChildEntities(), mNameId()
    {
    }

    void Entity::UpdateWorldTransform()
    {

        if (mParent && mParent->m_isTransformDirty)
        {
            m_isTransformDirty = true;
        }

        if (m_isTransformDirty)
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
                m_worldTransform.color.a = static_cast<unsigned char>(
                    (static_cast<int>(m_transform.color.a) * static_cast<int>(parentAlpha)) / 255);
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

    // Getter and setters

    void Entity::MarkTransformAsDirty()
    {
        if (m_isTransformDirty) // Optimization: if it's already dirty, there's no need to propagate it again.
        {
            return;
        }

        m_isTransformDirty = true;


        for (auto& child : mChildEntities) // Propagate the "dirty" state to all children, recursively
        {
            child->MarkTransformAsDirty();
        }
    }

    void Entity::SetPosition(float x, float y)
    {
        if (m_transform.position.x != x || m_transform.position.y != y)
        {
            MarkTransformAsDirty();
            m_transform.position.x = x;
            m_transform.position.y = y;
        }
    }

    Vector2f Entity::GetPosition() const
    {
        return Vector2f(m_transform.position.x, m_transform.position.y);
    }

    RectFloat& Entity::GetPositionRef()
    {
        MarkTransformAsDirty();
        return m_transform.position;
    }

    void Entity::SetSize(float width, float height)
    {
        if (m_transform.position.width != width || m_transform.position.height != height)
        {
            m_transform.position.width = width;
            m_transform.position.height = height;
            MarkTransformAsDirty();
        }
    }

    Sizef Entity::GetSize() const
    {
        return Sizef(m_transform.position.width, m_transform.position.height);
    }

    void Entity::SetSource(float x, float y, float width, float height)
    {
        if (m_transform.source.x != x || m_transform.source.x != y || m_transform.source.width != width ||
            m_transform.source.height != height)
        {
            m_transform.source.x = x;
            m_transform.source.y = y;
            m_transform.source.width = width;
            m_transform.source.height = height;
            MarkTransformAsDirty();
        }
    }

    RectFloat Entity::GetSource() const
    {
        return m_transform.source;
    }

    RectFloat& Entity::GetSourceRef()
    {
        MarkTransformAsDirty();
        return m_transform.source;
    }

    void Entity::SetOffset(float x, float y)
    {
        if (m_transform.offset.x != x || m_transform.offset.x != y)
        {
            MarkTransformAsDirty();
            m_transform.offset.x = x;
            m_transform.offset.y = y;
        }
    }

    Vector2f Entity::GetOffset() const
    {
        return Vector2f(m_transform.offset.x, m_transform.offset.y);
    }

    Vector2f& Entity::GetOffsetRef()
    {
        MarkTransformAsDirty();
        return m_transform.offset;
    }

    void Entity::SetOrigin(float x, float y)
    {
        if (m_transform.origin.x != x || m_transform.origin.x != y)
        {
            MarkTransformAsDirty();
            m_transform.origin.x = x;
            m_transform.origin.y = y;
        }
    }

    Vector2f Entity::GetOrigin() const
    {
        return Vector2f(m_transform.origin.x, m_transform.origin.y);
    }

    Vector2f& Entity::GetOriginRef()
    {
        MarkTransformAsDirty();
        return m_transform.origin;
    }

    void Entity::SetScale(float x, float y)
    {
        if (m_transform.scale.x != x || m_transform.scale.x != y)
        {
            MarkTransformAsDirty();
            m_transform.scale.x = x;
            m_transform.scale.y = y;
        }
    }

    Vector2f Entity::GetScale() const
    {
        return Vector2f(m_transform.scale.x, m_transform.scale.y);
    }

    Vector2f& Entity::GetScaleRef()
    {
        MarkTransformAsDirty();
        return m_transform.scale;
    }

    void Entity::SetRotation(float rotation)
    {
        if (m_transform.rotation != rotation)
        {
            MarkTransformAsDirty();
            m_transform.rotation = rotation;
        }
    }

    float Entity::GetRotation() const
    {
        return m_transform.rotation;
    }

    float& Entity::GetRotationRef()
    {
        MarkTransformAsDirty();
        return m_transform.rotation;
    }

    void Entity::SetColor(float r, float g, float b)
    {
        SetColor(r, g, b, m_transform.color.a);
    }

    void Entity::SetColor(float r, float g, float b, float a)
    {
        if (m_transform.color.r != r || m_transform.color.g != g || m_transform.color.b != b ||
            m_transform.color.a != a)
        {
            MarkTransformAsDirty();
            m_transform.color.r = r;
            m_transform.color.g = g;
            m_transform.color.b = b;
            m_transform.color.a = a;
        }
    }

    void Entity::SetColor(Color color)
    {
        SetColor(color.r, color.g, color.b, color.a);
    }

    void Entity::SetColorRed(float r)
    {
        SetColor(r, m_transform.color.g, m_transform.color.b, m_transform.color.a);
    }

    void Entity::SetColorGreen(float g)
    {
        SetColor(m_transform.color.r, g, m_transform.color.b, m_transform.color.a);
    }

    void Entity::SetColorBlue(float b)
    {
        SetColor(m_transform.color.r, m_transform.color.g, b, m_transform.color.a);
    }

    void Entity::SetOpacity(float a)
    {
        SetColor(m_transform.color.r, m_transform.color.g, m_transform.color.b, a);
    }

    Color Entity::GetColor() const
    {
        return m_transform.color;
    }

    Color& Entity::GetColorRef()
    {
        MarkTransformAsDirty();
        return m_transform.color;
    }

    Transform& Entity::GetTransformRef()
    {
        MarkTransformAsDirty();
        return m_transform;
    }

    Transform& Entity::GetWorldTransformRef()
    {
        MarkTransformAsDirty();
        return m_worldTransform;
    }


} // namespace ClassicLauncher
