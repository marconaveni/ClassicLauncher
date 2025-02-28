#include "GuiHorizontalBox.h"
#include "Application.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
    {
        mTransform.width = 10.0f;
        mTransform.height = 10.0f;
    }

    GuiHorizontalBox::~GuiHorizontalBox()
    {
    }

    void GuiHorizontalBox::AttachGui(EntityGui* guiComponent)
    {
        guiComponent->mTransform.x = 0;
        guiComponent->mTransform.y = 0;
        AddChild(guiComponent);
        mGuiElements.push_back(guiComponent);
    }

    void GuiHorizontalBox::ClearAll()
    {
        for (auto& guiComponent : mGuiElements)
        {
            guiComponent->RemoveParent();
        }
        RemoveAllChilds();
        mGuiElements.clear();
        mTransform.width = 10.0f;
        mTransform.height = 10.0f;
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
            Transform& transform = guiElement->mTransform;
            if (mIsAffectScale)
            {
                scale = transform.GetScale().x;    //transform.rootScaleX * transform.scaleX;
            }
            transform.x = x + width + mTransform.offset.x;
            transform.y = mTransform.offset.y;
            width += (transform.width + mSpacer) * scale;

            if (mIsAutoSize)
            {
                spacer += mSpacer;
                const float canvasScaleWidth = transform.GetScale().x; // (transform.rootScaleX * transform.scaleX);
                const float canvasScaleHeight = transform.GetScale().y; // (transform.rootScaleY * transform.scaleY);
                canvasWidth += (transform.width) / (canvasScaleWidth);
                canvasHeight = Math::Max(transform.height / canvasScaleHeight, canvasHeight);
            }
        }

        if (mIsAutoSize)
        {
            spacer -= mSpacer;
            mTransform.width = (canvasWidth + spacer / mTransform.GetScale().x) * scale;   //(canvasWidth + spacer / (mTransform.rootScaleX * mTransform.scaleX)) * scale;
            mTransform.height = canvasHeight * scale;
        }
    }

}  // namespace ClassicLauncher
