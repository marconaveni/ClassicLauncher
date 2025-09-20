#include "GuiHorizontalBox.h"

#include "Utils/Math.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
    {
        m_transform.position.width = 10.0f;
        m_transform.position.height = 10.0f;
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
        float width = 0;
        float canvasWidth = 0;
        float canvasHeight = 0;
        float scale = 1;
        float spacer = 0;

        for (auto& guiElement : mGuiElements)
        {
            Transform& transform = guiElement->m_transform;
            if (mIsAffectScale)
            {
                //scale = transform.GetScale().x; //transform.rootScaleX * transform.scaleX;
            }
            //transform.position.x = x + width + m_transform.offset.x;
            //transform.position.y = m_transform.offset.y;
            //width += (transform.position.width + mSpacer) * scale;

            if (mIsAutoSize)
            {
                spacer += mSpacer;
                transform.position.x = transform.position.width * x;
                x++;
                canvasWidth += transform.position.width;
                canvasHeight = transform.position.height > canvasHeight ? transform.position.height : canvasHeight;
                //const float canvasScaleWidth = transform.GetScale().x;  // (transform.rootScaleX * transform.scaleX);
                //const float canvasScaleHeight = transform.GetScale().y; // (transform.rootScaleY * transform.scaleY);
                //canvasWidth += (transform.position.width) / (canvasScaleWidth);
                //canvasHeight = Math::Max(transform.position.height / canvasScaleHeight, canvasHeight);
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
