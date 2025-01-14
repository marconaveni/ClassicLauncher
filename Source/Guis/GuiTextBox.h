#ifndef GUI_TEXT_BOX_H
#define GUI_TEXT_BOX_H

#include <string>
#include "Core.h"
#include "GuiComponent.h"

namespace ClassicLauncher
{

    class GuiComponent;

    class GuiTextBox : public GuiComponent
    {
    private:

        Font mFont;
        std::string mText;
        std::string mPathFont;
        int mSize;
        int mSpacing;
        Color mColor;

    public:

        GuiTextBox(const char* path, int size = 16, int spacing = 0);
        ~GuiTextBox() = default;
        void LoadNewFont(const char* path, int size = 16, int spacing = 0);
        virtual EntityType GetType() const override { return EntityType::GuiTextBox; };
        virtual void Update() override;
        virtual void Draw() override;
        virtual void End() override;
        void SetText(const char* text);
        void SetSize(int size);
        void SetSpacing(int spacing);
        Vector2 MeasureTextBox();
        const char* GetText() { return mText.c_str(); }
    };

}  // namespace ClassicLauncher

#endif