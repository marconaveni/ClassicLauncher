#include "GuiSizeBox.h"

#include "Helper.h"

namespace ClassicLauncher
{

    GuiSizeBox::GuiSizeBox(const EntityContext& entityContext)
        : GuiCanvas(entityContext)
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
        if (!m_entity)
        {
            return;
        }

        UpdateMargin();

        if (m_aspectRatio)
        {
            Vector2f newSize = m_entity->GetSize();
            m_entity->SetSize(newSize);
        }
        else
        {
            Vector2f newSize = GetSize();
            m_entity->SetSize(newSize);
        }
    }

    void GuiSizeBox::UpdateMargin()
    {

        Sizef offset{(GetSize().width - m_entity->GetSize().width) / 2, (GetSize().height - m_entity->GetSize().height) / 2};

        if (m_margin == Margin::LeftOnly)
        {
            offset.height = 0;
        }
        else if (m_margin == Margin::TopOnly)
        {
            offset.width = 0;
        }

        m_entity->SetOffset(offset);
    }

    void GuiSizeBox::SetMargin(Margin margin)
    {
        m_margin = margin;
    }

    void GuiSizeBox::SetAspectRatio(bool aspectRatio)
    {
        m_aspectRatio = aspectRatio;
    }


} // namespace ClassicLauncher
