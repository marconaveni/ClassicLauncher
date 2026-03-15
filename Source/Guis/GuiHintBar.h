#ifndef GUI_HINT_BAR
#define GUI_HINT_BAR

#include "Guis/Components/GuiHorizontalBox.h"
#include "Input/InputManager.h"

namespace ClassicLauncher
{

    class GuiTextBlock;
    class GuiBase;

    class GuiHintBar : public GuiHorizontalBox
    {
    public:

        GuiHintBar() = default;
        virtual EntityType GetType() const override { return EntityType::GuiHintBarClass; }
        void AddHint(RectFloat source, const std::string& name, Color textColor = Color::White, float spacer = 12.0f);
        void SetText(int position, const std::string& text);
        void SetVisibility(int position, bool enable);
        void SetTextColor(Color color);

    private:

        struct HintBar
        {
            GuiBase* icon{nullptr};
            GuiTextBlock* text{nullptr};
            GuiBase* spacer{nullptr};
        };

        std::vector<HintBar> m_hints{};
    };

} // namespace ClassicLauncher

#endif // GUI_HINT_BAR