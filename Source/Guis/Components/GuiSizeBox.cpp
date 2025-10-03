#include "GuiSizeBox.h"

namespace ClassicLauncher
{
    GuiSizeBox::GuiSizeBox()
        : m_entity(nullptr)
    {
    }

    void GuiSizeBox::AddChild(Entity* child)
    {
        if (!m_entity)
        {
            m_entity = child;
            GuiCanvas::AddChild(child);
        }     
    }

    void GuiSizeBox::RemoveChild(Entity* childEntity)
    {
         GuiCanvas::RemoveChild(childEntity);
         m_entity = nullptr;
    }

    void GuiSizeBox::Update()
    {
        Entity::Update();
        float x = (GetSize().width - m_entity->GetSize().width) / 2;
        float y = (GetSize().height - m_entity->GetSize().height) / 2;
        m_entity->SetOffset(x, y);
    }


} // namespace ClassicLauncher
