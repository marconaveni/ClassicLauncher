#include "GuiHorizontalBox.h"

#include "Utils/Math.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
    {
        m_transform.position.width = 10.0f;
        m_transform.position.height = 10.0f;
    }

    void GuiHorizontalBox::UpdateWorldTransform()
    {
        EntityGui::UpdateWorldTransform();
    }

    void GuiHorizontalBox::AttachGui(EntityGui* guiComponent)
    {
        guiComponent->m_transform.position.x = 0;
        guiComponent->m_transform.position.y = 0;
        AddChild(guiComponent);
        mGuiElements.push_back(guiComponent);
    }

    void GuiHorizontalBox::ClearAll()
    {
        for (auto& guiComponent : mGuiElements)
        {
            guiComponent->RemoveParent();
        }
        RemoveAllChildren();
        mGuiElements.clear();
        m_transform.position.width = 10.0f;
        m_transform.position.height = 10.0f;
    }

    void GuiHorizontalBox::Update()
    {
        EntityGui::Update();

        
        
        float x = 0;
        float canvasWidth = 0;
        float canvasHeight = 0;
        float spacer = 0;

        for (auto& guiElement : mGuiElements)
        {
            Transform& transform = guiElement->m_transform;
            Transform& worldTransform = guiElement->m_worldTransform;

            if (mIsAutoSize)
            {

                spacer += mSpacer;      
                transform.offset.x = (transform.position.width /* * m_worldTransform.scale.x */) * x ;
                x++;

                canvasWidth += transform.position.width;
                canvasHeight = transform.position.height > canvasHeight ? transform.position.height : canvasHeight;
            }
        }

        if (mIsAutoSize)
        {
           // spacer -= mSpace
           m_transform.position.width = canvasWidth;
           m_transform.position.height = canvasHeight; 
        }
    }


} // namespace ClassicLauncher
