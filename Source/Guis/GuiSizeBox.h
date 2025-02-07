#ifndef GUI_SIZE_BOX
#define GUI_SIZE_BOX

#include "Core.h"
#include "GuiComponent.h"

namespace ClassicLauncher
{

    class GuiSizeBox : public GuiComponent
    {
    private:

        GuiComponent* mGuiAttachment;
        bool mCropGuiAttachment;

    public:

        GuiSizeBox();
        ~GuiSizeBox();
        void AttachGui(GuiComponent* guiAttachment);
        virtual void Update() override;
        virtual void UpdatePosition() override;
        void UpdateGuiAttachment();
        void SetCropGuiAttachment(const bool bEnable) { mCropGuiAttachment = bEnable; }
    };

}  // namespace ClassicLauncher

#endif