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
        mTransform.width = 258.0f;
        mTransform.height = 282.0f;
        mTransform.sourceX = 771.0f;
        mTransform.sourceY = 0.0f;
    }

    GuiFrame::~GuiFrame()
    {
    }

    void GuiFrame::SetFrame(bool bForce)
    {
        std::vector<FocusComponent*> focusComponents = mFocusManager->GetAllFocusComponents();
        for (auto& focus : focusComponents)
        {
            if (focus->GetFocus())
            {
                Transformation target = mTransform;
                const float x = focus->GetEntity()->mTransform.position.x + focus->GetEntity()->mTransform.GetRootPosition().x;
                const float y = focus->GetEntity()->mTransform.position.y + focus->GetEntity()->mTransform.GetRootPosition().y;

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
        const int width = 256;
        const int height = 280;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transformation target = mTransform;

        target.scaleX = scale;
        target.scaleY = scale;

        target.position.x += (-width / 2 * target.scaleX) + width / 2;
        target.position.y += (-height / 2 * target.scaleY) + height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, mTransform, target, Ease::EaseQuadInOut, true);
        GetApplication()->GetTimerManager()->SetTimer(mTimer, [this]() { mTransform.color.a = 255; }, this, time * 2);
    }

    void GuiFrame::Update()
    {
        EntityGui::Update();
    }

}  // namespace ClassicLauncher