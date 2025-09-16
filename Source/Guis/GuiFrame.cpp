#include "GuiFrame.h"
#include "Application.h"
#include "Components/FocusComponent.h"
#include "Components/FocusManager.h"

namespace ClassicLauncher
{

    GuiFrame::GuiFrame(FocusManager* focusManager)
        : mFocusManager(focusManager)
    {
        mTextureName = "sprite";
        mTransform.position.width = 258.0f;
        mTransform.position.height = 282.0f;
        mTransform.source.x = 771.0f;
        mTransform.source.y = 0.0f;
    }

    void GuiFrame::SetFrame(bool bForce)
    {
        std::vector<FocusComponent*> focusComponents = mFocusManager->GetAllFocusComponents();
        for (auto& focus : focusComponents)
        {
            if (focus->GetFocus())
            {
                Transform target = mTransform;
                const float x = focus->GetEntity()->mTransform.position.x + focus->GetEntity()->mTransform.root.position.x;
                const float y = focus->GetEntity()->mTransform.position.y + focus->GetEntity()->mTransform.root.position.y;

                if (x == mTransform.position.x  && y == mTransform.position.y)
                {
                    return;
                }
                
                if (!GetAnimation("frame-move").GetAnimationIsRun() && !GetAnimation("card-zoom").GetAnimationIsRun())
                {
                    target.position.x = x;
                    target.position.y = y;
                    StartAnimation("frame-move", 0.15f, mTransform, target, Ease::EaseQuadInOut, false);
                }

            }
        }
    }

    void GuiFrame::Click()
    {
        const float width = 256;
        const float height = 280;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = mTransform;

        target.scale.x = scale;
        target.scale.y = scale;

        target.position.x += (-width / 2 * target.scale.x) + width / 2;
        target.position.y += (-height / 2 * target.scale.y) + height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, mTransform, target, Ease::EaseQuadInOut, true);
        GetTimerManager()->SetTimer(mTimer, [this]() { mTransform.color.a = 255; }, this, time * 2);
    }

    void GuiFrame::Update()
    {
        EntityGui::Update();
    }

}  // namespace ClassicLauncher