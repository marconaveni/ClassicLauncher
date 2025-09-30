#include "GuiBlackScreen.h"



namespace ClassicLauncher
{

    GuiBlackScreen::GuiBlackScreen()
    {
        m_transform.color.SetOpacity(0);
        m_transform.position.width = 1280;
        m_transform.position.height = 720;
        m_transform.source.width = 1280;
        m_transform.source.height = 720;
        mTextureName = "black";
    }

    void GuiBlackScreen::FadeIn()
    {
        m_transform.color.SetOpacity(0);
        Transform target = m_transform;
        target.color.a = 255;
        GetAnimationManager().StartAnimation("fade-in", 0.3f, &m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeOut()
    {
        m_transform.color.SetOpacity(255);
        Transform target = m_transform;
        target.color.a = 0;
        GetAnimationManager().StartAnimation("fade-out", 0.3f, &m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeInFadeOut()
    {
        m_transform.color.SetOpacity(0);
        Transform target = m_transform;
        target.color.a = 255;
        GetAnimationManager().StartAnimation("fade-in", 0.3f, &m_transform, target, Ease::EaseLinearNone, false);
        GetTimerManager()->SetTimer(m_timer, CALLFUNCTION(FadeOut, this), this, 1.8f);
    }

    void GuiBlackScreen::SetOpacity(int opacity)
    {
        m_transform.color.SetOpacity(opacity);
    }

    void GuiBlackScreen::Update()
    {
        Entity::Update();
        Animatable::UpdateAnimation();
    }


} // namespace  ClassicLauncher
