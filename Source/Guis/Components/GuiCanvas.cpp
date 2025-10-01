#include "GuiCanvas.h"


namespace ClassicLauncher
{


    void GuiCanvas::AddChild(Entity* entity)
    {
        Entity::AddChild(entity);
        m_entitiesPositions.push_back(EntityPosition{entity, Vector2f{}});
    }

    void GuiCanvas::Update()
    {
        Entity::Update();
    }

    void GuiCanvas::UpdateWorldTransform()
    {
        for (auto& entityPos : m_entitiesPositions)
        {
            entityPos.entity->SetPosition(Vector2f{});
        }

        Entity::UpdateWorldTransform();
    }

    // void GuiCanvas::SetOffset(Entity* entity, Vector2f position)
    // {
    //     SetOffset(entity, position.x, position.y);
    // }

    // void GuiCanvas::SetOffset(Entity* entity, float x, float y)
    // {
    //     for (auto& entityPos : m_entitiesPositions)
    //     {
    //         if (entityPos.entity == entity)
    //         {
    //             entityPos.entity->m_transform.offset.x = x;
    //             entityPos.entity->m_transform.offset.y = y;
    //             break;
    //         }
    //     }
    // }

    // Vector2f GuiCanvas::GetOffset(Entity* entity) const
    // {
    //     for (auto& entityPos : m_entitiesPositions)
    //     {
    //         if (entityPos.entity == entity)
    //         {
    //             return entityPos.entity->m_transform.offset;
    //         }
    //     }
    //     return Vector2f();
    // }


} // namespace ClassicLauncher
