#include "GuiButton.h"

#include "Entity/EntityManager.h"
#include "Guis/GuiBase.h"

namespace ClassicLauncher
{
    GuiButton::GuiButton(FocusManager* focusManagerRef)
        : FocusComponent(focusManagerRef)
    {
    }

    GuiButton::~GuiButton()
    {
    }

    void GuiButton::Init(const Vector2f sourceIcon)
    {
        SetSize({96.0f, 69.0f});
        m_background = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
        m_background->SetSize(GetSize());
        m_background->SetSource(Vector2f{1139.0f, 0.0f}, GetSize());
        m_background->SetOpacity(0);
        m_background->m_textureName = "sprite";
        AddChild(m_background);

        m_icon = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
        m_icon->SetSize(GetSize());
        m_icon->SetSource(sourceIcon, GetSize());
        m_icon->m_textureName = "sprite";
        AddChild(m_icon);
    }

    void GuiButton::Update()
    {
        GuiCanvas::Update();
        Animatable::UpdateAnimation();
    }

    void GuiButton::OnFocus()
    {
    }

    void GuiButton::OnLostFocus()
    {
    }

} // namespace ClassicLauncher
