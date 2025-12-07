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

        GuiMenu() = default;
        virtual EntityType GetType() const override { return EntityType::GuiMenuClass; }
        void Init();
        void Update() override;

    private:

        struct Buttons
        {
            GuiButton* button{nullptr};
            bool active{true};
        };
        
        void CreateButton(const Vector2f source);
        std::vector<Buttons> m_buttons{};
        GuiBase* m_background{nullptr};
        
    };

} // namespace ClassicLauncher

#endif // GUI_MENU_H