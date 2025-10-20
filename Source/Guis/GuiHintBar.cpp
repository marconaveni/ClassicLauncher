#include "GuiHintBar.h"

#include "Entity/EntityManager.h"
#include "Guis/GuiBase.h"
#include "Guis/GuiTextBlock.h"
#include "Utils/Resources.h"


namespace ClassicLauncher
{

    void GuiHintBar::AddHint(RectFloat source, const std::string& name, Color textColor, float spacer)
    {
        HintBar bar;
        bar.icon = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
        bar.text = GetEntityManager()->CreateEntity<GuiTextBlock>("GuiTextBlock");
        bar.spacer = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");

        bar.icon->m_textureName = "sprite";
        bar.icon->SetSize(source.GetSize());
        bar.icon->SetSource(source);

        bar.text->LoadNewFont(Resources::GetFontFile(), 24, 0);
        bar.text->SetText(name);
        bar.text->SetOffset({0, 7});
        bar.text->SetAutoSize(true);
        bar.text->SetColor(textColor);

        bar.spacer->SetSize({spacer, 3});

        AttachGui(bar.icon);
        AttachGui(bar.text);
        AttachGui(bar.spacer);

        AddChild(bar.icon);
        AddChild(bar.text);
        AddChild(bar.spacer);

        SetSpace(2);
        m_hints.push_back(bar);
    }

    void GuiHintBar::SetText(int position, const std::string& text)
    {
        if (m_hints.size() > 0 && position < m_hints.size())
        {
            m_hints[position].text->SetText(text);
        }
    }

    void GuiHintBar::SetVisibility(int position, bool enable)
    {
        if (m_hints.size() > 0 && position < m_hints.size())
        {
            m_hints[position].icon->SetVisible(enable);
            m_hints[position].text->SetVisible(enable);
            m_hints[position].spacer->SetVisible(enable);
        }
    }

    void GuiHintBar::SetTextColor(Color color)
    {
        for (auto hint : m_hints)
        {
            hint.text->SetColor(color);
        }
    }

} // namespace ClassicLauncher