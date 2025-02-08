#include "GuiHorizontalBox.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
    {
        mProperties.width = 10.0f;
        mProperties.height = 10.0f;
    }

    GuiHorizontalBox::~GuiHorizontalBox()
    {
    }

    void GuiHorizontalBox::AttachGui(EntityGui* guiComponent)
    {
        AddChild(guiComponent);
        mGuiElements.push_back(guiComponent);
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

        for (auto& guiComponent : mGuiElements)
        {
            TransformProperties& properties = guiComponent->mProperties;
            if (mIsAffectScale)
            {
                scale = properties.rootScaleX * properties.scaleX;
            }
            properties.x = x + width + mProperties.offset.x;
            properties.y = mProperties.offset.y;
            width += (properties.width + mSpacer) * scale;

            if (mIsAutoSize)
            {
                spacer += mSpacer;
                const float canvasScaleWidth = (properties.rootScaleX * properties.scaleX);
                const float canvasScaleHeight = (properties.rootScaleY * properties.scaleY);
                canvasWidth += (properties.width) / (canvasScaleWidth);
                canvasHeight = Math::Max(properties.height / canvasScaleHeight, canvasHeight);
            }
        }

        if (mIsAutoSize)
        {
            spacer -= mSpacer;
            mProperties.width = (canvasWidth + spacer / (mProperties.rootScaleX * mProperties.scaleX)) * scale;
            mProperties.height = canvasHeight * scale;
        }
    }

}  // namespace ClassicLauncher
