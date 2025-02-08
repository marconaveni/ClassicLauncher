#ifndef GUI_HORIZONTAL_BOX
#define GUI_HORIZONTAL_BOX

#include <vector>
#include "Entity/EntityGui.h"

namespace ClassicLauncher
{

    class GuiComponent;

    class GuiHorizontalBox : public EntityGui
    {
    private:

        std::vector<EntityGui*> mGuiElements;
        bool mIsAffectScale = false;
        float mSpacer = 0;
        bool mIsAutoSize = false;

    public:

        GuiHorizontalBox();
        ~GuiHorizontalBox();
        virtual EntityType GetType() const override { return EntityType::GuiHorizontalBoxClass; }
        void SetAffectScale(bool bIsAffectScale) { mIsAffectScale = bIsAffectScale; }
        void SetSpace(float spacer) { mSpacer = spacer; }
        void SetAutoSize(bool bIsAutoSize) { mIsAutoSize = bIsAutoSize; }
        void AttachGui(EntityGui* guiComponent);
        virtual void Update() override;
    };

}  // namespace ClassicLauncher

#endif