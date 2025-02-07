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

    void GuiSizeBox::AttachGui(GuiComponent* guiAttachment)
    {
        mGuiAttachment = guiAttachment;
        AddChild(mGuiAttachment);
    }

    void GuiSizeBox::Update()
    {
        GuiComponent::Update();
        UpdateGuiAttachment();
        if (mCropGuiAttachment)
        {
            EnableScissorMode(mDestination.x, mDestination.y, mProperties.width, mProperties.height);
        }
    }

    void GuiSizeBox::UpdatePosition()
    {
        GuiComponent::UpdatePosition();
    }

    void GuiSizeBox::UpdateGuiAttachment()
    {
        if (mGuiAttachment == nullptr)
        {
            return;
        }
        mGuiAttachment->mProperties.x = (mProperties.width * mProperties.rootScaleX - mGuiAttachment->mProperties.width * mGuiAttachment->mProperties.rootScaleX) / 2;
        mGuiAttachment->mProperties.y = (mProperties.height * mProperties.rootScaleY - mGuiAttachment->mProperties.height * mGuiAttachment->mProperties.rootScaleY) / 2;
    }

}  // namespace ClassicLauncher
