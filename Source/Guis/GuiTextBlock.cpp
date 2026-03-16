#include "GuiTextBlock.h"

#include "Graphics/FontManager.h"
#include "Helper.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Window/Window.h"
#include "rl_wrap.h"


namespace ClassicLauncher
{
    void GuiTextBlock::LoadNewFont(const std::string& path, int size, int spacing)
    {
        m_pathFont = path;
        m_sizeText = size;
        m_spacing = spacing;
        UpdateFont(path);
    }

    void GuiTextBlock::UpdateFont(const std::string& path)
    {
        m_renderScale = ThemesManager::GetScaleRenderer();
        m_nameFont = GetFontManager()->Load(path, m_sizeText * m_renderScale);
    }

    GuiTextBlock::GuiTextBlock()
    {
    }

    GuiTextBlock::~GuiTextBlock()
    {
    }

    void GuiTextBlock::Init(const std::string& path, int size, int spacing)
    {
        LoadNewFont(path, size, spacing);
    }

    void GuiTextBlock::Update()
    {
        Entity::Update();

        if (m_autoSize)
        {
            // SetSize(m_mensuredText.x, m_mensuredText.y);
            return;
        }


        if (m_centerText && GetSize().width > GetMeasureTextBox().x)
        {
            const float widthPosition = GetSize().width * GetWorldTransform().scale.x * GetScale().x;
            const float widthText = GetMeasureTextBox().x * GetWorldTransform().scale.x * GetScale().x;
            m_positionText = (widthPosition * m_renderScale - widthText * m_renderScale) / 2.0f;
        }
        else
        {
            m_positionText = 0.0f;
        }

        const float positionText = (GetSize().width - m_mensuredText.x) * m_renderScale;
        if (m_textOverflowPolicy == TextOverflowPolicy::Clip && positionText < 0)
        {
            if (m_delay < m_maxDelay)
            {
                m_delay += GetWindow()->GetFrameTime();
                return;
            }
            LOG(LogClassicTrace, "positionText %.2f m_offsetText %.2f", positionText, m_offsetText);

            m_speed = 0.50f * 60.0f * GetWindow()->GetFrameTime();

            if (positionText - m_offsetText > 0 || positionText - m_offsetText < positionText)
            {
                m_delay = 0.0f;
                m_isLeft = !m_isLeft;
            }

            m_offsetText += (m_isLeft) ? -m_speed : m_speed;
        }
    }

    void GuiTextBlock::Draw()
    {

        Font* font = GetFontManager()->GetFont(m_nameFont);

        if (!font || !IsVisible())
        {
            return;
        }

        RectFloat finalTransformRect = m_finalRender.transform;

        if (m_textOverflowPolicy == TextOverflowPolicy::Clip)
        {
            rlw::BeginScissorMode(finalTransformRect.x, finalTransformRect.y, finalTransformRect.width, finalTransformRect.height);
        }
        rlw::DrawTextEx(*font,
                        m_text.data(),
                        Vector2f{finalTransformRect.x + m_positionText + m_offsetText, finalTransformRect.y}, // Vector2f{300 , 400},
                        m_sizeText * GetWorldTransform().scale.y * m_renderScale,
                        m_spacing * m_renderScale,
                        GetColor());
        if (m_textOverflowPolicy == TextOverflowPolicy::Clip)
        {
            rlw::EndScissorMode();
        }
    }

    void GuiTextBlock::End()
    {
    }

    void GuiTextBlock::SetText(const std::string& text)
    {
        m_text = text;
        m_offsetText = 0;
        m_delay = 1;

        m_mensuredText = MeasureTextBox();
        const float x = m_autoSize ? m_mensuredText.x : GetSize().width;
        SetSize(x, m_mensuredText.y);
    }

    void GuiTextBlock::SetAutoSize(bool enable)
    {
        m_autoSize = enable;
        SetSize(m_mensuredText.x, m_mensuredText.y);
    }

    void GuiTextBlock::SetSizeFont(int size)
    {
        LoadNewFont(m_pathFont, size, m_spacing);
    }

    void GuiTextBlock::SetSpacing(int spacing)
    {
        m_spacing = spacing;
    }

    Vector2f GuiTextBlock::GetMeasureTextBox()
    {
        return m_mensuredText;
    }

    Vector2f GuiTextBlock::MeasureTextBox()
    {
        Font* font = GetFontManager()->GetFont(m_nameFont);
        if (!font)
        {
            return Vector2f{};
        }

        return font->MeasureTextEx(m_text, m_sizeText, m_spacing);
        // return m_font.MeasureTextEx(m_text, m_sizeText, m_spacing);
    }

    void GuiTextBlock::SetTextOverflowPolicy(TextOverflowPolicy textOverflowPolicy)
    {
        m_textOverflowPolicy = textOverflowPolicy;
    }

    void GuiTextBlock::SetOffSetMoveText(float speed, float maxDelay)
    {
        m_speed = speed;
        m_maxDelay = maxDelay;
    }

} // namespace ClassicLauncher
