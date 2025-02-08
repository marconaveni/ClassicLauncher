#ifndef GUI_BLACK_SCREEN_H
#define GUI_BLACK_SCREEN_H


#include "Entity/EntityGui.h"

namespace ClassicLauncher
{
    class EntityGui;

    class GuiBlackScreen : public EntityGui
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