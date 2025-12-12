#include "GuiButton.h"

#include "Entity/EntityManager.h"
#include "Guis/GuiBase.h"

namespace ClassicLauncher
{
    GuiButton::GuiButton(FocusManager* focusManagerRef)
        : FocusComponent(focusManagerRef, FocusCategory::BUTTON_ICON)
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
        m_background->SetOpacity(255);
        Transform target = m_icon->GetTransform();
        //target.scale = 1.1f;



        const float scale = 1.1f;

        target.scale.x = scale * m_icon->GetTransform().scale.x;
        target.scale.y = scale * m_icon->GetTransform().scale.y;

        const float width = target.position.width * m_icon->GetTransform().scale.x;
        const float height = target.position.height * m_icon->GetTransform().scale.y;

        target.offset.x += (-width / 2 * scale) + width / 2;
        target.offset.y += (-height / 2 * scale) + height / 2;

        GetAnimationManager().StartAnimation("focus", 0.15f, m_icon, target, Ease::EaseLinearNone, false);
    }

    void GuiButton::OnLostFocus()
    {
        m_background->SetOpacity(0);
        Transform target = m_icon->GetTransform();
        target.scale = 1.0f;
        target.offset.x = 0;
        target.offset.y = 0;
        GetAnimationManager().StartAnimation("focus", 0.15f, m_icon, target, Ease::EaseLinearNone, false);
    }

} // namespace ClassicLauncher
