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
        virtual EntityType GetType() const override { return EntityType::GuiBlackScreenClass; }
        void FadeIn();
        void FadeOut();
        void FadeInFadeOut();
        void KeepBlack();
        void SetOpacity(int opacity);
        virtual void Update() override;
        virtual void AnimationFinished(std::string name) override;
    };

}  // namespace ClassicLauncher

#endif  // GUI_BLACK_SCREEN_H