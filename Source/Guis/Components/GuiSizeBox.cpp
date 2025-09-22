#include "GuiSizeBox.h"

namespace ClassicLauncher
{
    GuiSizeBox::GuiSizeBox()
        : mGuiAttachment(nullptr), mCropGuiAttachment(false)
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
           // EnableScissorMode(m_transform.GetTransform().x,
           //                   m_transform.GetTransform().y,
           //                   m_transform.position.width,
           //                   m_transform.position.height);
        }
    }

    void GuiSizeBox::UpdateWorldTransform()
    {
        EntityGui::UpdateWorldTransform();
    }

    void GuiSizeBox::UpdateGuiAttachment()
    {
        if (mGuiAttachment == nullptr)
        {
            return;
        }

        // todo refactor to decrease line size
        //mGuiAttachment->m_transform.position.x = (m_transform.position.width * m_transform.root.scale.x - mGuiAttachment->m_transform.position.width * mGuiAttachment->m_transform.root.scale.x) / 2;
        //mGuiAttachment->m_transform.offset.x = m_transform.offset.x; 
        //mGuiAttachment->m_transform.position.y = (m_transform.position.height * m_transform.root.scale.y - mGuiAttachment->m_transform.position.height * mGuiAttachment->m_transform.root.scale.y) / 2;
        //mGuiAttachment->m_transform.offset.y = m_transform.offset.y;
    }

} // namespace ClassicLauncher
