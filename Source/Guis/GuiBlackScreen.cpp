#include "GuiBlackScreen.h"
#include "Application.h"

namespace ClassicLauncher
{
    GuiBlackScreen::GuiBlackScreen()
    {
        mTransform.color.SetOpacity(0);
        mTransform.scaleWidth = 1280;
        mTransform.scaleHeight = 720;
        mTextureName = "black";
    }

    void GuiBlackScreen::FadeIn()
    {
        mTransform.color.SetOpacity(0);
        Transform target = mTransform;
        target.color.a = 255;
        StartAnimation("fade-in", 0.3f, mTransform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeOut()
    {
        mTransform.color.SetOpacity(255);
        Transform target = mTransform;
        target.color.a = 0;
        StartAnimation("fade-out", 0.3f, mTransform, target, Ease::EaseQuadOut, false);
    }
    
    void GuiBlackScreen::FadeInFadeOut()
    {
        mTransform.color.SetOpacity(0);
        Transform target = mTransform;
        target.color.a = 255;
        StartAnimation("fade-in-out", 0.3f, mTransform, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::KeepBlack()
    {        
        mTransform.color.SetOpacity(255);
        Transform target = mTransform;   
        StartAnimation("keep", 1.0f, mTransform, target, Ease::EaseLinearNone, false);

    }

    void GuiBlackScreen::SetOpacity(int opacity)
    {
        mTransform.color.SetOpacity(opacity);
    }

    void GuiBlackScreen::Update()
    {
        EntityGui::Update();
    }
    
    void GuiBlackScreen::AnimationFinished(std::string name)
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
}  // namespace  ClassicLauncher
