#ifndef GUI_SIZE_BOX
#define GUI_SIZE_BOX

#include "Entity/EntityGui.h"

namespace ClassicLauncher
{

    class EntityGui;

    class GuiSizeBox : public EntityGui
    {
    public:

        GuiSizeBox();
        ~GuiSizeBox() = default;
        virtual EntityType GetType() const override { return EntityType::GuiSizeBoxClass; }
        void AttachGui(EntityGui* guiAttachment);
        virtual void Update() override;
        virtual void UpdateWorldTransform() override;
        void UpdateGuiAttachment();
        void SetCropGuiAttachment(const bool bEnable) { mCropGuiAttachment = bEnable; }

    private:

        EntityGui* mGuiAttachment;
        bool mCropGuiAttachment;
    };

} // namespace ClassicLauncher

#endif