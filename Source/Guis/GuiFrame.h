#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "Entity/EntityGui.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{
    class FocusManager;

    class GuiFrame : public EntityGui
    {
    public:

        GuiFrame(FocusManager* focusManager);
        ~GuiFrame() = default;
        void SetFrame(bool bForce = false);
        void Click();
        virtual EntityType GetType() const override { return EntityType::GuiFrameClass; }
        virtual void Update() override;

    private:

        FocusManager* mFocusManager;
        TimerHandling mTimer;
    };

}  // namespace ClassicLauncher

#endif