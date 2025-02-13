#include "GuiFrame.h"
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

    void GuiFrame::SetFrame(float clampMin, float clampMax)
    {
        std::vector<FocusComponent*> focusComponents = mFocusManager->GetAllFocusComponents();

        for (auto& focus : focusComponents)
        {
            if (focus->GetFocus())
            {
                Transform target = mTransform;    
                target.x = focus->GetEntity()->mTransform.x + focus->GetEntity()->mTransform.GetRootPosition().x;
                target.y = focus->GetEntity()->mTransform.y + focus->GetEntity()->mTransform.GetRootPosition().y;
                if (clampMin > 0 && clampMax > 0)
                {
                    target.x = Math::Clamp(target.x, clampMin, clampMax);
                }
                StartAnimation("frame-move", 0.2f, mTransform, target, Ease::EaseQuadInOut, false);
                return;
            }
        }

    }

    void GuiFrame::Update()
    {
        EntityGui::Update();
    }

}  // namespace ClassicLauncher