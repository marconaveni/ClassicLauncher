#include "GuiFrame.h"


#include "Components/FocusComponent.h"
#include "Components/FocusManager.h"

namespace ClassicLauncher
{

    GuiFrame::GuiFrame(FocusManager* focusManager)
        : mFocusManager(focusManager)
    {
        mTextureName = "sprite";
        m_transform.position.width = 258.0f;
        m_transform.position.height = 282.0f;
        m_transform.source.x = 771.0f;
        m_transform.source.y = 0.0f;
    }

    void GuiFrame::SetFrame(bool bForce)
    {
        std::vector<FocusComponent*> focusComponents = mFocusManager->GetAllFocusComponents();
        for (auto& focus : focusComponents)
        {
            //if (focus->GetFocus())
            //{
            //    Transform target = m_transform;
            //    const float x = focus->GetEntity()->m_transform.position.x + focus->GetEntity()->m_transform.root.position.x;
            //    const float y = focus->GetEntity()->m_transform.position.y + focus->GetEntity()->m_transform.root.position.y;
//
            //    if (x == m_transform.position.x && y == m_transform.position.y)
            //    {
            //        return;
            //    }
//
            //    if (!GetAnimation("frame-move").GetAnimationIsRun() && !GetAnimation("card-zoom").GetAnimationIsRun())
            //    {
            //        target.position.x = x;
            //        target.position.y = y;
            //        StartAnimation("frame-move", 0.15f, m_transform, target, Ease::EaseQuadInOut, false);
            //    }
            //}
        }
    }

    void GuiFrame::Click()
    {
        const float width = 256;
        const float height = 280;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = m_transform;

        target.scale.x = scale;
        target.scale.y = scale;

        target.position.x += (-width / 2 * target.scale.x) + width / 2;
        target.position.y += (-height / 2 * target.scale.y) + height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, m_transform, target, Ease::EaseQuadInOut, true);
        GetTimerManager()->SetTimer(mTimer, [this]() { m_transform.color.a = 255; }, this, time * 2);
    }

    void GuiFrame::Update()
    {
        EntityGui::Update();
    }

} // namespace ClassicLauncher