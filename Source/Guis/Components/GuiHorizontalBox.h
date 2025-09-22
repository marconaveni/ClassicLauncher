#ifndef GUI_HORIZONTAL_BOX
#define GUI_HORIZONTAL_BOX

#include <vector>

#include "Guis/Components/GuiCanvas.h"

namespace ClassicLauncher
{

    class GuiHorizontalBox : public GuiCanvas
    {
    public:

        GuiHorizontalBox();
        ~GuiHorizontalBox() = default;
        virtual EntityType GetType() const override { return EntityType::GuiHorizontalBoxClass; }
        virtual void UpdateWorldTransform() override;
        void SetAffectScale(bool bIsAffectScale) { mIsAffectScale = bIsAffectScale; }
        void SetSpace(float spacer) { mSpacer = spacer; }
        void SetAutoSize(bool bIsAutoSize) { mIsAutoSize = bIsAutoSize; }
        void AttachGui(EntityGui* guiComponent);
        void ClearAll();
        virtual void Update() override;

    private:

        std::vector<EntityGui*> mGuiElements;
        bool mIsAffectScale = false;
        float mSpacer = 0;
        bool mIsAutoSize = false;
    };

} // namespace ClassicLauncher

#endif