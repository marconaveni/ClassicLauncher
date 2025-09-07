#include "GuiHorizontalBox.h"
#include "Application.h"
#include "Utils/Math.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
    {
        mTransform.position.width = 10.0f;
        mTransform.position.height = 10.0f;
    }

    GuiHorizontalBox::~GuiHorizontalBox()
    {
    }

    void GuiHorizontalBox::AttachGui(EntityGui* guiComponent)
    {
        guiComponent->mTransform.position.x = 0;
        guiComponent->mTransform.position.y = 0;
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
        mTransform.position.width = 10.0f;
        mTransform.position.height = 10.0f;
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
            Transformation& transform = guiElement->mTransform;
            if (mIsAffectScale)
            {
                scale = transform.GetScale().x;    //transform.rootScaleX * transform.scaleX;
            }
            transform.position.x = x + width + mTransform.offset.x;
            transform.position.y = mTransform.offset.y;
            width += (transform.position.width + mSpacer) * scale;

            if (mIsAutoSize)
            {
                spacer += mSpacer;
                const float canvasScaleWidth = transform.GetScale().x; // (transform.rootScaleX * transform.scaleX);
                const float canvasScaleHeight = transform.GetScale().y; // (transform.rootScaleY * transform.scaleY);
                canvasWidth += (transform.position.width) / (canvasScaleWidth);
                canvasHeight = Math::Max(transform.position.height / canvasScaleHeight, canvasHeight);
            }
        }

        if (mIsAutoSize)
        {
            spacer -= mSpacer;
            mTransform.position.width = (canvasWidth + spacer / mTransform.GetScale().x) * scale;   //(canvasWidth + spacer / (mTransform.rootScaleX * mTransform.scaleX)) * scale;
            mTransform.position.height = canvasHeight * scale;
        }
    }

}  // namespace ClassicLauncher
