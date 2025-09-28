#ifndef GUI_SIZE_BOX
#define GUI_SIZE_BOX

#include "Entity/Entity.h"

namespace ClassicLauncher
{



    class GuiSizeBox : public Entity
    {
    public:

        GuiSizeBox();
        ~GuiSizeBox() = default;
        virtual EntityType GetType() const override { return EntityType::GuiSizeBoxClass; }
        void AttachGui(Entity* guiAttachment);
        virtual void Update() override;
        virtual void UpdateWorldTransform() override;
        void UpdateGuiAttachment();
        void SetCropGuiAttachment(const bool bEnable) { mCropGuiAttachment = bEnable; }

    private:

        Entity* mGuiAttachment;
        bool mCropGuiAttachment;
    };

} // namespace ClassicLauncher

#endif