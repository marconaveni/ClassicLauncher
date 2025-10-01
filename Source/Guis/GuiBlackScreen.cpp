#include "GuiBlackScreen.h"



namespace ClassicLauncher
{

    GuiBlackScreen::GuiBlackScreen()
    {
        SetOpacity(0);
        SetSize(Sizef{1280.0f, 720.0f});
        SetSource(0.0f, 0.0f, 1280.0f, 720.0f);
        //m_transform.color.SetOpacity(0);
        //m_transform.position.width = 1280;
        //m_transform.position.height = 720;
        //m_transform.source.width = 1280;
        //m_transform.source.height = 720;
        mTextureName = "black";
    }

    void GuiBlackScreen::FadeIn()
    {
        SetOpacity(0);
        Transform target = GetTransform();
        target.color.a = 255;
        GetAnimationManager().StartAnimation("fade-in", 0.3f, this, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeOut()
    {
        SetOpacity(255);
        Transform target = GetTransform();
        target.color.a = 0;
        GetAnimationManager().StartAnimation("fade-out", 0.3f, this, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeInFadeOut()
    {
        SetOpacity(0);
        Transform target = GetTransform();
        target.color.a = 255;
        GetAnimationManager().StartAnimation("fade-in", 0.3f, this, target, Ease::EaseLinearNone, false);
        GetTimerManager()->SetTimer(m_timer, CALLFUNCTION(FadeOut, this), this, 1.8f);
    }

    void GuiBlackScreen::Update()
    {
        Entity::Update();
        Animatable::UpdateAnimation();
    }


} // namespace  ClassicLauncher
