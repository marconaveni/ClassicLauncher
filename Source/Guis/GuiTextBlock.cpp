#include "GuiTextBlock.h"
#include "Application.h"
#include "Utils/Math.h"

namespace ClassicLauncher
{
    void GuiTextBlock::LoadNewFont(const std::string& path, int size, int spacing)
    {
        mPathFont = path;
        mSize = size;
        mSpacing = spacing;
        UpdateFont(path);
    }

    void GuiTextBlock::UpdateFont(const std::string& path)
    {
        if (rlw::IsFontValid(mFont))
        {
            rlw::UnloadFont(mFont);
            mFont = rlw::Font();
        }
        const float scale = Themes::GetScaleTexture();
        mFont = rlw::LoadFontEx(path.data(), mSize * scale, NULL, 250);
    }

    GuiTextBlock::GuiTextBlock(const std::string& path, int size, int spacing)
        : mFont()
        , mText()
        , mPathFont()
        , mSize()
        , mSpacing()
        , mColor(Color::White)
        , mTextMovement(false)
        , mDesiredWidth(10)
        , mOffset(0)
        , mToLeft(true)
        , mDelay(0)
        , mMensuredText(Vector2f())
        , mSpeed(0.5f)
        , mMaxDelay(3.0f)
        , mTextOverflowPolicy(TextOverflowPolicy::none)
    {
        LoadNewFont(path, size, spacing);
        mTextureName = "text";
    }

    GuiTextBlock::~GuiTextBlock()
    {
        UnloadText();
    }

    void GuiTextBlock::Update()
    {
        EntityGui::Update();

        if (mTextOverflowPolicy == TextOverflowPolicy::clip)
        {
            EnableScissorMode(mTransform.GetTransform().x, mTransform.GetTransform().y, mDesiredWidth, mMensuredText.y);
        }

        const int positionText = mDesiredWidth - (int)mMensuredText.x;
        if (mTextOverflowPolicy == TextOverflowPolicy::clip && positionText < 0)
        {
            if (mDelay < mMaxDelay)
            {
                mDelay += rlw::GetFrameTime();
                return;
            }

            mSpeed = 0.50f * 60.0f * rlw::GetFrameTime();

            if (positionText - mOffset > 0 || positionText - mOffset < positionText)
            {
                mDelay = 0.0f;
                mToLeft = !mToLeft;
            }

            mOffset += (mToLeft) ? -mSpeed : mSpeed;
        }
    }

    void GuiTextBlock::Draw()
    {
        mColor.a = mTransform.color.a;
        Vector2f posi = { mTransform.GetTransform().x, mTransform.GetTransform().y };
        posi.x += mOffset * Themes::GetScaleTexture();
        Vector2f scale = mTransform.GetScale();
        rlw::DrawTextEx(mFont, mText.data(), posi, mSize * Math::Max(scale.x * Themes::GetScaleTexture(), scale.y * Themes::GetScaleTexture()), mSpacing, mColor);
    }

    void GuiTextBlock::End()
    {
        UnloadText();
    }

    void GuiTextBlock::SetText(const std::string& text)
    {
        mText = text;
        mOffset = 0;
        mDelay = 1;

        mMensuredText = MeasureTextBox();
        mTransform.source.width = mMensuredText.x;
        mTransform.source.height = mMensuredText.y;
    }

    void GuiTextBlock::SetSize(int size)
    {
        LoadNewFont(mPathFont.c_str(), size, mSpacing);
    }

    void GuiTextBlock::SetSpacing(int spacing)
    {
        mSpacing = spacing;
    }

    void GuiTextBlock::SetColor(Color tint)
    {
        mColor = tint;
    }

    Vector2f GuiTextBlock::GetMeasureTextBox()
    {
        return mMensuredText;
    }

    Vector2f GuiTextBlock::MeasureTextBox()
    {
        return rlw::MeasureTextEx(mFont, mText.c_str(), mSize, mSpacing);
    }

    void GuiTextBlock::SetTextOverflowPolicy(TextOverflowPolicy textOverflowPolicy)
    {
        mTextOverflowPolicy = textOverflowPolicy;
    }

    void GuiTextBlock::SetDesiredWidth(int newWidth)
    {
        mDesiredWidth = newWidth;
    }

    void GuiTextBlock::SetOffSetMoveText(float speed, float maxDelay)
    {
        mSpeed = speed;
        mMaxDelay = maxDelay;
    }

    void GuiTextBlock::UnloadText()
    {
        if (rlw::IsFontValid(mFont))
        {
            rlw::UnloadFont(mFont);
            mFont = rlw::Font();
        }
    }

}  // namespace ClassicLauncher
