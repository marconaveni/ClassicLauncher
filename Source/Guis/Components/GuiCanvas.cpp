#include "GuiCanvas.h"


namespace ClassicLauncher
{

    GuiCanvas::GuiCanvas(const EntityContext& entityContext)
        : Entity(entityContext)
    {
    }

    void GuiCanvas::AddChild(Entity* entity)
    {
        Entity::AddChild(entity);
        m_entities.push_back(entity);
    }

    void GuiCanvas::Update()
    {
        Entity::Update();
    }

    void GuiCanvas::UpdateWorldTransform()
    {
        for (auto& entity : m_entities)
        {
            entity->SetPosition(Vector2f{});
        }
        Entity::UpdateWorldTransform();
    }

} // namespace ClassicLauncher
