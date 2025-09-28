#ifndef GUI_BLACK_SCREEN_H
#define GUI_BLACK_SCREEN_H

#include "Animations/Animatable.h"
#include "Entity/Entity.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{

    class GuiBlackScreen : public Entity, public Animatable
    {

    public:

        GuiBlackScreen();
        virtual EntityType GetType() const override { return EntityType::GuiBlackScreenClass; }
        void FadeIn();
        void FadeOut();
        void FadeInFadeOut();
        void SetOpacity(int opacity);
        virtual void Update() override;

    private:

        TimerHandling m_timer{};
    };

} // namespace ClassicLauncher

#endif // GUI_BLACK_SCREEN_H