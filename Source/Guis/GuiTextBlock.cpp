#include "GuiTextBlock.h"

#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"

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
        if (mFont.IsValid())
        {
            mFont.Unload();
            mFont = Font();
        }
        const float scale = ThemesManager::GetScaleRenderer();
        mFont.LoadFromFile(path.data(), mSize * scale, nullptr, 250);
    }

    GuiTextBlock::GuiTextBlock(const std::string& path, int size, int spacing)
        : mFont()
        , mSize()
        , mSpacing()
        , mColor(Color::White)
        , mTextMovement(false)
        , mDesiredWidth(10)
        , mOffset(0)
        , mToLeft(true)
        , mDelay(0)
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
            //EnableScissorMode(m_transform.GetTransform().x, m_transform.GetTransform().y, mDesiredWidth, mMensuredText.y);
        }

        const int positionText = mDesiredWidth - (int)mMensuredText.x;
        if (mTextOverflowPolicy == TextOverflowPolicy::clip && positionText < 0)
        {
            if (mDelay < mMaxDelay)
            {
                mDelay += RayWindow::GetFrameTime();
                return;
            }

            mSpeed = 0.50f * 60.0f * RayWindow::GetFrameTime();

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
        //mColor.a = m_transform.color.a;
        //Vector2f posi = Vector2f{m_transform.GetTransform().x, m_transform.GetTransform().y};
        //posi.x += mOffset * ThemesManager::GetScaleTexture();
        //Vector2f scale = m_transform.GetScale();
        //rlw::DrawTextEx(
        //    mFont,
        //    mText.data(),
        //    posi,
        //    mSize * Math::Max(scale.x * ThemesManager::GetScaleTexture(), scale.y * ThemesManager::GetScaleTexture()),
        //    mSpacing,
        //    mColor);
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
        m_transform.source.width = mMensuredText.x;
        m_transform.source.height = mMensuredText.y;
    }

    void GuiTextBlock::SetSize(int size)
    {
        LoadNewFont(mPathFont, size, mSpacing);
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
        return mFont.MeasureTextEx(mText, mSize, mSpacing);
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
        if (mFont.IsValid())
        {
            mFont.Unload();
            mFont = Font();
        }
    }

} // namespace ClassicLauncher
