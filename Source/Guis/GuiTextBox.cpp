#include "GuiTextBox.h"
#include "Application.h"

namespace ClassicLauncher
{
    void GuiTextBox::LoadNewFont(const std::string& path, int size, int spacing)
    {
        mPathFont = path;
        mSize = size;
        mSpacing = spacing;
        UpdateFont(path);
    }

    void GuiTextBox::UpdateFont(const std::string& path)
    {
        if (IsFontValid(mFont))
        {
            UnloadFont(mFont);
            mFont = Font();
        }
        const float scale = Themes::GetScaleTexture();
        mFont = LoadFontEx(path.data(), mSize * scale, NULL, 250);
    }

    GuiTextBox::GuiTextBox(const std::string& path, int size, int spacing)
        : mFont(Font{ 0 })
        , mText()
        , mPathFont()
        , mSize()
        , mSpacing()
        , mColor(Color::White())
        , mTextMovement(false)
        , mDesiredWidth(10)
        , mOffset(0)
        , mToLeft(true)
        , mDelay(0)
        , mMensuredText(Vector2())
        , mSpeed(0.5f)
        , mMaxDelay(240.0f)
        , mTextOverflowPolicy(TextOverflowPolicy::none)
    {
        LoadNewFont(path, size, spacing);
    }

    GuiTextBox::~GuiTextBox()
    {
        UnloadText();
    }

    void GuiTextBox::Update()
    {
        GuiComponent::Update();

        const int positionText = mDesiredWidth - mMensuredText.GetIntX();
        if (mTextOverflowPolicy == TextOverflowPolicy::clip && positionText < 0)
        {
            mSpeed = 0.50f;

            if (mDelay != 0 && mDelay < mMaxDelay)
            {
                mDelay++;
                return;
            }

            if ((abs)(positionText - mOffset) <= 0 || (mOffset > 0))
            {
                mDelay = 1;
                mToLeft = !mToLeft;
            }
            else
            {
                mDelay = 0;
            }

            mOffset += (mToLeft) ? -mSpeed : mSpeed;
        }
    }

    void GuiTextBox::Draw()
    {
        Vector2 posi = Vector2{ mProperties.x + mProperties.rootX + mOffset, mProperties.y + mProperties.rootY };
        const float scale = Themes::GetScaleTexture();
        posi.x = posi.x * scale;
        posi.y = posi.y * scale;
        mColor.a = mProperties.color.a;
        DrawTextEx(mFont, mText.data(), posi, mSize * scale, mSpacing, mColor);
    }

    void GuiTextBox::End()
    {
        UnloadText();
    }

    void GuiTextBox::SetText(const std::string& text)
    {
        mText = text;
        mOffset = 0;
        mDelay = 1;

        mMensuredText = MeasureTextBox();
        mProperties.scaleWidth = mMensuredText.x;
        mProperties.scaleHeight = mMensuredText.y;

        if (mTextOverflowPolicy == TextOverflowPolicy::clip)
        {
            const float scale = Themes::GetScaleTexture();
            EnableScissorMode(0, 0, mDesiredWidth * scale, mMensuredText.y * scale);
        }
        else if (mTextOverflowPolicy == TextOverflowPolicy::none)
        {
            DisableScissorMode();
        }
    }

    void GuiTextBox::SetSize(int size)
    {
        LoadNewFont(mPathFont.c_str(), size, mSpacing);
    }

    void GuiTextBox::SetSpacing(int spacing)
    {
        mSpacing = spacing;
    }

    void GuiTextBox::SetColor(Color tint)
    {
        mColor = tint;
    }

    Vector2 GuiTextBox::GetMeasureTextBox()
    {
        return mMensuredText;
    }

    Vector2 GuiTextBox::MeasureTextBox()
    {
        return MeasureTextEx(mFont, mText.c_str(), mSize, mSpacing);
    }

    void GuiTextBox::SetTextOverflowPolicy(TextOverflowPolicy textOverflowPolicy)
    {
        mTextOverflowPolicy = textOverflowPolicy;
    }

    void GuiTextBox::SetDesiredWidth(int newWidth)
    {
        mDesiredWidth = newWidth;
    }

    void GuiTextBox::SetOffSetMoveText(float speed, float maxDelay)
    {
        mSpeed = speed;
        mMaxDelay = maxDelay;
    }

    void GuiTextBox::UnloadText()
    {
        if (IsFontValid(mFont))
        {
            UnloadFont(mFont);
            mFont = Font();
        }
    }

}  // namespace ClassicLauncher
