#include "GuiTextBox.h"

namespace ClassicLauncher
{
    void GuiTextBox::LoadNewFont(const char* path, int size, int spacing)
    {
        if (IsFontValid(mFont))
        {
            UnloadFont(mFont);
        }
        mPathFont = path;
        mSize = size;
        mSpacing = spacing;
        mFont = LoadFontEx(path, size, NULL, 0);
    }

    GuiTextBox::GuiTextBox(const char* path, int size, int spacing)
        : mColor(Color::White())
    {
        LoadNewFont(path, size, spacing);
    }

    void GuiTextBox::Update()
    {
        GuiComponent::Update();
        mProperties.scaleWidth = MeasureTextBox().GetIntX();
        mProperties.scaleHeight = MeasureTextBox().GetIntY();
    }

    void GuiTextBox::Draw()
    {
        Vector2 posi = Vector2{ mProperties.x + mProperties.rootX, mProperties.y + mProperties.rootY };
        DrawTextEx(mFont, mText.c_str(), posi, mSize, mSpacing, mColor);
    }

    void GuiTextBox::End()
    {
        if (IsFontValid(mFont))
        {
            UnloadFont(mFont);
        }
    }

    void GuiTextBox::SetText(const char* text)
    {
        mText = text;
    }

    void GuiTextBox::SetSize(int size)
    {
        LoadNewFont(mPathFont.c_str(), size, mSpacing);
    }

    void GuiTextBox::SetSpacing(int spacing)
    {
        mSpacing = spacing;
    }

    Vector2 GuiTextBox::MeasureTextBox()
    {
        return MeasureTextEx(mFont, mText.c_str(), mSize, mSpacing);
    }

}  // namespace ClassicLauncher
