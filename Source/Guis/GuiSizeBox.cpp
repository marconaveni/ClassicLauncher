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
            EnableScissorMode(mTransform.GetTransform().x, mTransform.GetTransform().y, mTransform.width, mTransform.height);
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
        mGuiAttachment->mTransform.x = (mTransform.width * mTransform.rootScaleX - mGuiAttachment->mTransform.width * mGuiAttachment->mTransform.rootScaleX) / 2;
        mGuiAttachment->mTransform.offset.x = mTransform.offset.x; 
        mGuiAttachment->mTransform.y = (mTransform.height * mTransform.rootScaleY - mGuiAttachment->mTransform.height * mGuiAttachment->mTransform.rootScaleY) / 2;
        mGuiAttachment->mTransform.offset.y = mTransform.offset.y;
    }

}  // namespace ClassicLauncher
