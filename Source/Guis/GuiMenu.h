#ifndef GUI_MENU_H
#define GUI_MENU_H

#include "Guis/Components/GuiHorizontalBox.h"

namespace ClassicLauncher
{

    class GuiBase;
    class GuiButton;

    class GuiMenu : public GuiHorizontalBox
    {
    public:

        explicit GuiMenu(const EntityContext& entityContext);
        virtual EntityType GetType() const override { return EntityType::GuiMenuClass; }
        void Init();
        void Update() override;
        void SetButtonFocus(int id);
        void EnableButton(bool isActive, int id);

    private:

        struct MenuButtons
        {
            GuiButton* button{nullptr};
            std::string textName{};
            bool active{true};
        };

        void CreateButton(const Vector2f source, const std::string& textName, bool isActive = true);
        void EnableButton(bool isActive, MenuButtons& menuButton);
        void UpdateSize();
        std::vector<MenuButtons> m_menuButtons{};
        GuiBase* m_background{nullptr};
        int m_id{0};
        bool m_isNeedUpdateSize{false};
    };

} // namespace ClassicLauncher

#endif // GUI_MENU_H