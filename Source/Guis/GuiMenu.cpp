#include "GuiMenu.h"

#include "Entity/EntityManager.h"
#include "Guis/GuiButton.h"
#include "Input/InputManager.h"
#include "Utils/Math.h"

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

        if (Keyboard::IsDown(Keyboard::Key::C))
        {
            m_buttons[0].button->SetFocus();
        }
    }

    void GuiMenu::SetButtonFocus(int id)
    {
        int idPrevious = m_id;
        m_id += id;
        if (m_id < 0)
        {
            m_id = m_buttons.size() - 1;
        }
        else if (m_id > m_buttons.size() - 1)
        {
            m_id = 0;
        }

        m_buttons[m_id].button->SetFocus();
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