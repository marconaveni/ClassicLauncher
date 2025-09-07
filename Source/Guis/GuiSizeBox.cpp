#include "GuiSizeBox.h"

namespace ClassicLauncher
{
    GuiSizeBox::GuiSizeBox()
        : mGuiAttachment(nullptr), mCropGuiAttachment(false)
    {
    }

    GuiSizeBox::~GuiSizeBox()
    {
    }

    void GuiSizeBox::AttachGui(EntityGui* guiAttachment)
    {
        mGuiAttachment = guiAttachment;
        AddChild(mGuiAttachment);
    }

    void GuiSizeBox::Update()
    {
        EntityGui::Update();
        UpdateGuiAttachment();
        if (mCropGuiAttachment)
        {
            EnableScissorMode(mTransform.GetTransform().x, mTransform.GetTransform().y, mTransform.position.width, mTransform.position.height);
        }
    }

    void GuiSizeBox::UpdatePosition()
    {
        EntityGui::UpdatePosition();
    }

    void GuiSizeBox::UpdateGuiAttachment()
    {
        if (mGuiAttachment == nullptr)
        {
            return;
        }
        mGuiAttachment->mTransform.position.x = (mTransform.position.width * mTransform.root.scale.x - mGuiAttachment->mTransform.position.width * mGuiAttachment->mTransform.root.scale.x) / 2;
        mGuiAttachment->mTransform.offset.x = mTransform.offset.x; 
        mGuiAttachment->mTransform.position.y = (mTransform.position.height * mTransform.root.scale.y - mGuiAttachment->mTransform.position.height * mGuiAttachment->mTransform.root.scale.y) / 2;
        mGuiAttachment->mTransform.offset.y = mTransform.offset.y;
    }

}  // namespace ClassicLauncher
