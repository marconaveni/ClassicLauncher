#include "GuiFrame.h"
#include "Components/FocusComponent.h"
#include "Components/FocusManager.h"
#include "Application.h"

namespace ClassicLauncher
{
    GuiFrame::GuiFrame(FocusManager* focusManager)
        : mFocusManager(focusManager)
    {
        mTextureName = "sprite";
        mTransform.width = 258.0f;
        mTransform.height = 282.0f;
        mTransform.sourceX = 771.0f;
        mTransform.sourceY = 0.0f;
    }

    GuiFrame::~GuiFrame()
    {
    }

    void GuiFrame::SetFrame(float clampXMin, float clampXMax, float clampYMin, float clampYMax)
    {
        std::vector<FocusComponent*> focusComponents = mFocusManager->GetAllFocusComponents();

        for (auto& focus : focusComponents)
        {
            if (focus->GetFocus())
            {
                Transform target = mTransform;
                target.x = focus->GetEntity()->mTransform.x + focus->GetEntity()->mTransform.GetRootPosition().x;
                target.y = focus->GetEntity()->mTransform.y + focus->GetEntity()->mTransform.GetRootPosition().y;
                target.x = Math::Clamp(target.x, clampXMin, clampXMax);
                target.y = Math::Clamp(target.y, clampYMin, clampYMax);
                StartAnimation("frame-move", 0.2f, mTransform, target, Ease::EaseQuadInOut, false);
                return;
            }
        }
    }

    void GuiFrame::Click()
    {
        const int width = 256;
        const int height = 280;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = mTransform;

        target.scaleX = scale;
        target.scaleY = scale;

        target.x += (-width / 2 * target.scaleX) + width / 2;
        target.y += (-height / 2 * target.scaleY) + height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, mTransform, target, Ease::EaseQuadInOut, true);
        GetApplication()->GetTimerManager()->SetTimer(
            mTimer,
            [this]()
            {
                mTransform.color.a = 255;
            },
            this,
            time * 2);
    }

    void GuiFrame::Update()
    {
        EntityGui::Update();
    }

}  // namespace ClassicLauncher