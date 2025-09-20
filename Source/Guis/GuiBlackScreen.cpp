#include "GuiBlackScreen.h"



namespace ClassicLauncher
{
    GuiBlackScreen::GuiBlackScreen()
    {
        m_transform.color.SetOpacity(0);
        m_transform.source.width = 1280;
        m_transform.source.height = 720;
        mTextureName = "black";
    }

    void GuiBlackScreen::FadeIn()
    {
        m_transform.color.SetOpacity(0);
        Transform target = m_transform;
        target.color.a = 255;
        StartAnimation("fade-in", 0.3f, m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeOut()
    {
        m_transform.color.SetOpacity(255);
        Transform target = m_transform;
        target.color.a = 0;
        StartAnimation("fade-out", 0.3f, m_transform, target, Ease::EaseQuadOut, false);
    }

    void GuiBlackScreen::FadeInFadeOut()
    {
        m_transform.color.SetOpacity(0);
        Transform target = m_transform;
        target.color.a = 255;
        StartAnimation("fade-in-out", 0.3f, m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::KeepBlack()
    {
        m_transform.color.SetOpacity(255);
        Transform target = m_transform;
        StartAnimation("keep", 1.0f, m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::SetOpacity(int opacity)
    {
        m_transform.color.SetOpacity(opacity);
    }

    void GuiBlackScreen::Update()
    {
        EntityGui::Update();
    }

    void GuiBlackScreen::AnimationFinished(const std::string& name)
    {
        if (name == "fade-in-out")
        {
            KeepBlack();
        }
        if (name == "keep")
        {
            FadeOut();
        }
    }
} // namespace  ClassicLauncher
