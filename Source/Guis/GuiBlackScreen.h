#ifndef GUI_BLACK_SCREEN_H
#define GUI_BLACK_SCREEN_H

#include "Core.h"
#include "GuiComponent.h"

namespace ClassicLauncher
{
    class GuiComponent;

    class GuiBlackScreen : public GuiComponent
    {
    public:

        GuiBlackScreen();
        void FadeIn();
        void FadeOut();
        void FadeInFadeOut();
        void KeepBlack();
        virtual void Update() override;
        virtual void AnimationFinished(std::string name) override;

    };

}  // namespace ClassicLauncher

#endif  // GUI_BLACK_SCREEN_H