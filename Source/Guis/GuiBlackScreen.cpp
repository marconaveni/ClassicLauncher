#include "GuiBlackScreen.h"
#include "Application.h"

namespace ClassicLauncher
{
    GuiBlackScreen::GuiBlackScreen()
    {
        mProperties.color.SetOpacity(0);
        mProperties.scaleWidth = 1280;
        mProperties.scaleHeight = 720;
        mTextureName = "black";
    }

    void GuiBlackScreen::FadeIn()
    {
        mProperties.color.SetOpacity(0);
        TransformProperties target = mProperties;
        target.color.a = 255;
        StartAnimation("fade-in", 0.3f, mProperties, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::FadeOut()
    {
        mProperties.color.SetOpacity(255);
        TransformProperties target = mProperties;
        target.color.a = 0;
        StartAnimation("fade-out", 0.3f, mProperties, target, Ease::EaseQuadOut, false);
    }
    
    void GuiBlackScreen::FadeInFadeOut()
    {
        mProperties.color.SetOpacity(0);
        TransformProperties target = mProperties;
        target.color.a = 255;
        StartAnimation("fade-in-out", 0.3f, mProperties, target, Ease::EaseLinearNone, false);
    }

    void GuiBlackScreen::KeepBlack()
    {        
        mProperties.color.SetOpacity(255);
        TransformProperties target = mProperties;   
        StartAnimation("keep", 1.0f, mProperties, target, Ease::EaseLinearNone, false);

    }

    void GuiBlackScreen::SetOpacity(int opacity)
    {
        mProperties.color.SetOpacity(opacity);
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
