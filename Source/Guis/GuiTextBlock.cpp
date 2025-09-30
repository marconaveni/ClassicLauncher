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
        m_renderScale = ThemesManager::GetScaleRenderer();
        mFont.LoadFromFile(path.data(), mSize * m_renderScale, nullptr, 250);
    }

    GuiTextBlock::GuiTextBlock(const std::string& path, int size, int spacing)
        : mFont()
        , mSize(size)
        , mSpacing(spacing)
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
        Entity::Update();

        if (m_centerText && m_transform.position.width > GetMeasureTextBox().x)
        {
            const float widthPosition = m_transform.position.width * m_worldTransform.scale.x * m_transform.scale.x;
            const float widthText = GetMeasureTextBox().x * m_worldTransform.scale.x * m_transform.scale.x;
            m_positionText = (widthPosition * m_renderScale - widthText * m_renderScale) / 2.0f;
        }
        else
        {
            m_positionText = 0.0f;
        }


        const int positionText = m_transform.position.width - (int)mMensuredText.x;
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

        RectFloat finalTransformRect = m_finalRender.transform;

        if (mTextOverflowPolicy == TextOverflowPolicy::clip)
        {
            rlw::BeginScissorMode(finalTransformRect.x,
                                  finalTransformRect.y,
                                  finalTransformRect.width,
                                  finalTransformRect.height);
        }
        rlw::DrawTextEx(
            mFont,
            mText.data(),
            Vector2f{finalTransformRect.x + m_positionText + mOffset, finalTransformRect.y}, // Vector2f{300 , 400},
            mSize * m_worldTransform.scale.y * m_renderScale,
            mSpacing * m_renderScale,
            mColor);
        if (mTextOverflowPolicy == TextOverflowPolicy::clip)
        {
            rlw::EndScissorMode();
        }
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
        //m_transform.position.width = mMensuredText.x;
        m_transform.position.height = mMensuredText.y;
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
