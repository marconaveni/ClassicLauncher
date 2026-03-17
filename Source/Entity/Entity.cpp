#include "Entity.h"

#include <algorithm>

#include "ClassicAssert.h"

namespace ClassicLauncher
{

    Entity::Entity(const EntityContext& entityContext)
        : m_entityContext(entityContext)
    {
    }

    void Entity::UpdateWorldTransform()
    {

        if (m_parent && m_parent->m_isTransformDirty)
        {
            m_isTransformDirty = true;
        }

        if (m_isTransformDirty)
        {
            if (m_parent)
            {
                const float parentX = m_parent->m_worldTransform.position.x;
                const float parentY = m_parent->m_worldTransform.position.y;


                m_worldTransform.position.x = parentX + (m_transform.position.x * m_parent->m_worldTransform.scale.x);
                m_worldTransform.position.y = parentY + (m_transform.position.y * m_parent->m_worldTransform.scale.y);

                // apply scale
                m_worldTransform.scale.x = m_parent->m_worldTransform.scale.x * m_transform.scale.x;
                m_worldTransform.scale.y = m_parent->m_worldTransform.scale.y * m_transform.scale.y;

                // apply offset
                m_worldTransform.offset.x = m_parent->m_worldTransform.offset.x + m_transform.offset.x;
                m_worldTransform.offset.y = m_parent->m_worldTransform.offset.y + m_transform.offset.y;

                m_worldTransform.rotation = m_parent->m_worldTransform.rotation + m_transform.rotation;

                // It transmits the opacity (alpha) from the father to the son.
                unsigned char parentAlpha = m_parent->m_worldTransform.color.a;
                m_worldTransform.color.a = static_cast<unsigned char>((static_cast<int>(m_transform.color.a) * static_cast<int>(parentAlpha)) / 255);
            }
            else
            {
                m_worldTransform = m_transform;
            }

            for (auto& entity : m_childEntities)
            {
                entity->UpdateWorldTransform();
            }
        }
    }

    void Entity::SelfDelete()
    {
        m_isCanDelete = true;
        for (auto& entity : m_childEntities)
        {
            entity->SelfDelete();
        }
    }

    void Entity::AddChild(Entity* childEntity)
    {
        if (childEntity->m_parent != this)
        {
            childEntity->m_parent = this;
            m_childEntities.emplace_back(childEntity);
        }
    }

    void Entity::RemoveChild(Entity* childEntity)
    {
        m_childEntities.erase(std::remove_if(m_childEntities.begin(),
                                             m_childEntities.end(),
                                             [childEntity](const Entity* entity)
                                             {
                                                 return entity == childEntity; // Return true element
                                             }),
                              m_childEntities.end());
    }

    void Entity::RemoveAllChildren()
    {
        m_childEntities.clear();
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
        return m_childEntities;
    }

    Entity* Entity::GetRootEntity()
    {
        Entity* rootParent = m_parent;
        if (rootParent != nullptr)
        {
            rootParent = rootParent->GetRootEntity();
        }
        return (rootParent != nullptr) ? rootParent : this;
    }

    void Entity::EnableScissorMode(float x, float y, float width, float height)
    {
        m_isScissorMode = true;
        m_scissorArea = RectFloat{x, y, width, height};
        for (auto& childEntity : m_childEntities)
        {
            childEntity->EnableScissorMode(m_scissorArea.x, m_scissorArea.y, m_scissorArea.width, m_scissorArea.height);
        }
    }

    // Manager Getters

    TimerManager* Entity::GetTimerManager()
    {
        CLASSIC_ASSERT(m_entityContext.timerManager, "This pointer is null. Verify m_entityContext has valid pointers");
        return m_entityContext.timerManager;
    }

    SpriteManager* Entity::GetSpriteManager()
    {
        CLASSIC_ASSERT(m_entityContext.spriteManager, "This pointer is null. Verify m_entityContext has valid pointers");
        return m_entityContext.spriteManager;
    }

    EntityManager* Entity::GetEntityManager()
    {
        CLASSIC_ASSERT(m_entityContext.entityManager, "This pointer is null. Verify m_entityContext has valid pointers");
        return m_entityContext.entityManager;
    }

    FocusManager* Entity::GetFocusManager()
    {
        CLASSIC_ASSERT(m_entityContext.focusManager, "This pointer is null. Verify m_entityContext has valid pointers");
        return m_entityContext.focusManager;
    }

    FontManager* Entity::GetFontManager()
    {
        CLASSIC_ASSERT(m_entityContext.fontManager, "This pointer is null. Verify m_entityContext has valid pointers");
        return m_entityContext.fontManager;
    }

    Window* Entity::GetWindow()
    {
        CLASSIC_ASSERT(m_entityContext.window, "This pointer is null. Verify m_entityContext has valid pointers");
        return m_entityContext.window;
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


        for (auto& child : m_childEntities) // Propagate the "dirty" state to all children, recursively
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
        if (m_transform.source.x != x || m_transform.source.x != y || m_transform.source.width != width || m_transform.source.height != height)
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

    void Entity::SetOffset(float x, float y)
    {
        if (m_transform.offset.x != x || m_transform.offset.y != y)
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

    void Entity::SetOrigin(float x, float y)
    {
        if (m_transform.origin.x != x || m_transform.origin.y != y)
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

    void Entity::SetScale(float x, float y)
    {
        if (m_transform.scale.x != x || m_transform.scale.y != y)
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

    void Entity::SetColor(float r, float g, float b)
    {
        SetColor(r, g, b, m_transform.color.a);
    }

    void Entity::SetColor(float r, float g, float b, float a)
    {
        if (m_transform.color.r != r || m_transform.color.g != g || m_transform.color.b != b || m_transform.color.a != a)
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
