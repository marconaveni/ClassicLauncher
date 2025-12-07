#include "GuiMenu.h"
#include "Entity/EntityManager.h"
#include "Guis/GuiButton.h"

namespace ClassicLauncher
{

    void GuiMenu::Init()
    {
        CreateButton(Vector2f{1109, 196});
        CreateButton(Vector2f{1206, 196});
        CreateButton(Vector2f{1303, 196});
        CreateButton(Vector2f{1400, 196});
        CreateButton(Vector2f{1497, 196});
    }

    void GuiMenu::Update()
    {
        GuiHorizontalBox::Update();
    }

    void GuiMenu::CreateButton(const Vector2f source)
    {
        Buttons button;
        button.button = GetEntityManager()->CreateEntity<GuiButton>("GuiButton", GetFocusManager());
        button.button->Init(source);
        AttachGui(button.button);
        AddChild(button.button);
        m_buttons.push_back(button);
        const float x = button.button->GetSize().width * m_buttons.size();
        const float y = button.button->GetSize().height;
        SetSize(x, y);
    }


} // namespace ClassicLauncher