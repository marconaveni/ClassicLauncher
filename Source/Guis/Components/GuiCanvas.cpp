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




} // namespace ClassicLauncher
