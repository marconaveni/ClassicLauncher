#include "GuiTextBlock.h"
#include "Application.h"

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
        if (IsFontValid(mFont))
        {
            UnloadFont(mFont);
            mFont = Font();
        }
        const float scale = Themes::GetScaleTexture();
        mFont = LoadFontEx(path.data(), mSize * scale, NULL, 250);
    }

    GuiTextBlock::GuiTextBlock(const std::string& path, int size, int spacing)
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
        , mMaxDelay(3.0f)
        , mTextOverflowPolicy(TextOverflowPolicy::none)
    {
        LoadNewFont(path, size, spacing);
    }

    GuiTextBlock::~GuiTextBlock()
    {
        UnloadText();
    }

    void GuiTextBlock::Update()
    {
        GuiComponent::Update();

        if (mTextOverflowPolicy == TextOverflowPolicy::clip)
        {
            EnableScissorMode(mDestination.x, mDestination.y, mDesiredWidth, mMensuredText.y);
        }

        const int positionText = mDesiredWidth - mMensuredText.GetIntX();
        if (mTextOverflowPolicy == TextOverflowPolicy::clip && positionText < 0)
        {
            if (mDelay < mMaxDelay)
            {
                mDelay += GetFrameTime();
                return;
            }
            
            mSpeed = 0.50f * 60.0f * GetFrameTime();

            if (positionText - mOffset > 0 ||  positionText - mOffset < positionText)
            {
                mDelay = 0.0f;
                mToLeft = !mToLeft;
            }

            mOffset += (mToLeft) ? -mSpeed : mSpeed;
        }
    }

    void GuiTextBlock::Draw()
    {
        Vector2 posi = Vector2{ mProperties.x + mProperties.rootX + mOffset, mProperties.y + mProperties.rootY };
        const float scale = Themes::GetScaleTexture();

        const Vector2 scaleS = {scale * mProperties.scaleX * mProperties.rootScaleX, scale * mProperties.scaleY * mProperties.rootScaleY}; 
        posi.x = posi.x * scale;
        posi.y = posi.y * scale;
        mColor.a = mProperties.color.a;
        DrawTextEx(mFont, mText.data(), posi, mSize * Math::Max(scaleS.x, scaleS.y), mSpacing, mColor);
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
        mProperties.scaleWidth = mMensuredText.x;
        mProperties.scaleHeight = mMensuredText.y;



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

    Vector2 GuiTextBlock::GetMeasureTextBox()
    {
        return mMensuredText;
    }

    Vector2 GuiTextBlock::MeasureTextBox()
    {
        return MeasureTextEx(mFont, mText.c_str(), mSize, mSpacing);
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
        if (IsFontValid(mFont))
        {
            UnloadFont(mFont);
            mFont = Font();
        }
    }

}  // namespace ClassicLauncher
