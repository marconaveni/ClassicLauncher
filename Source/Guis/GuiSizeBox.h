#ifndef GUI_SIZE_BOX
#define GUI_SIZE_BOX


#include "GuiComponent.h"

namespace ClassicLauncher
{

    class EntityGui;

    class GuiSizeBox : public EntityGui
    {
    private:

        GuiComponent* mGuiAttachment;
        bool mCropGuiAttachment;

    public:

        GuiSizeBox();
        ~GuiSizeBox();
        virtual EntityType GetType() const override { return EntityType::GuiSizeBoxClass; }
        void AttachGui(GuiComponent* guiAttachment);
        virtual void Update() override;
        virtual void UpdatePosition() override;
        void UpdateGuiAttachment();
        void SetCropGuiAttachment(const bool bEnable) { mCropGuiAttachment = bEnable; }
    };

}  // namespace ClassicLauncher

#endif