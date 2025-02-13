#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include <vector>
#include "Entity/EntityGui.h"


namespace ClassicLauncher
{
    class FocusManager;

    class GuiFrame : public EntityGui
    {
    private:

        std::vector<Entity*> mFollowEntities;
        FocusManager* mFocusManager;

    public:

        GuiFrame(FocusManager* focusManager);
        ~GuiFrame();
        void SetFrame(float clampMin = 0, float clampMax = 0);
        virtual EntityType GetType() const override { return EntityType::GuiFrameClass; }
        virtual void Update() override;
    };

}  // namespace ClassicLauncher

#endif