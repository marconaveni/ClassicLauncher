#include "GuiMenu.h"

#include "Entity/EntityManager.h"
#include "Guis/GuiButton.h"
#include "Input/InputManager.h"
#include "Utils/Math.h"

namespace ClassicLauncher
{
    GuiMenu::GuiMenu(const EntityContext& entityContext)
        : GuiHorizontalBox(entityContext)
    {
    }

    void GuiMenu::Init()
    {
        CreateButton(Vector2f{1109, 196}, "Display", true);
        CreateButton(Vector2f{1206, 196}, "Settings", true);
        CreateButton(Vector2f{1303, 196}, "Scrapper", false);
        CreateButton(Vector2f{1400, 196}, "Info", false);
        CreateButton(Vector2f{1497, 196}, "Search", false);
    }

    void GuiMenu::Update()
    {
        GuiHorizontalBox::Update();
        UpdateSize();
    }

    void GuiMenu::SetButtonFocus(int id)
    {
        m_id += id;
        if (m_id < 0)
        {
            m_id = m_menuButtons.size() - 1;
        }
        else if (m_id > m_menuButtons.size() - 1)
        {
            m_id = 0;
        }

        if (!m_menuButtons[m_id].active)
        {
            SetButtonFocus((id == 0) ? 1 : id);
            return;
        }
        if (!m_menuButtons[m_id].button->IsFocus())
        {
            m_menuButtons[m_id].button->SetFocus();
        }
    }

    void GuiMenu::CreateButton(const Vector2f source, const std::string& textName, bool isActive)
    {
        MenuButtons menuButton;
        menuButton.button = GetEntityManager()->CreateEntity<GuiButton>("GuiButton", GetFocusManager(), GetWindow());
        menuButton.button->Init(source);
        EnableButton(isActive, menuButton);

        AttachGui(menuButton.button);
        AddChild(menuButton.button);

        m_menuButtons.push_back(menuButton);
    }


    void GuiMenu::EnableButton(bool isActive, int id)
    {
        EnableButton(isActive, m_menuButtons.at(id));
    }

    void GuiMenu::EnableButton(bool isActive, MenuButtons& menuButton)
    {
        menuButton.active = isActive;
        const Sizef size{96.0f, 69.0f};
        menuButton.button->SizeButton(isActive ? size : Sizef{0});
        menuButton.button->SetOpacity(isActive ? 255 : 0);
        m_isNeedUpdateSize = true;
    }

    void GuiMenu::UpdateSize()
    {
        if (m_isNeedUpdateSize)
        {

            float x = 0.0f;
            float y = 0.0f;
            for (const auto& menuButton : m_menuButtons)
            {
                if (menuButton.active)
                {
                    x += menuButton.button->GetSize().width * m_menuButtons.size();
                    y = menuButton.button->GetSize().height;
                }
            }
            SetSize(x, y);
            m_isNeedUpdateSize = false;
        }
    }


} // namespace ClassicLauncher