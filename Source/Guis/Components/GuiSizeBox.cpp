#include "GuiSizeBox.h"

#include "Helper.h"
#include "Utils/Utils.h"
#include "Utils/Math.h"

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
        if (!m_entity)
        {
            return;
        }

        UpdateMargin();

        if (m_bAspectRatio)
        {
            Vector2f newSize = m_entity->GetSize();
            //Utils::SetSizeWithProportionFit(newSize, GetSize().width, GetSize().height);
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

        Sizef offset{(GetSize().width - m_entity->GetSize().width) / 2,
                     (GetSize().height - m_entity->GetSize().height) / 2};

        if (m_margin == Margin::LEFT_ONLY)
        {
            offset.height = 0;
        }
        else if (m_margin == Margin::TOP_ONLY)
        {
            offset.width = 0;
        }

        m_entity->SetOffset(offset);
    }

    void GuiSizeBox::SetMargin(Margin margin)
    {
        m_margin = margin;
    }

    void GuiSizeBox::SetAspectRatio(bool bAspectRatio)
    {
        m_bAspectRatio = bAspectRatio;
    }


} // namespace ClassicLauncher
