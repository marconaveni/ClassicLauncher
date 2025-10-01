#include "GuiFrame.h"

#include "Components/FocusComponent.h"
#include "Components/FocusManager.h"
#include "Helper.h"
#include "Utils/Math.h"

namespace ClassicLauncher
{

    GuiFrame::GuiFrame(FocusManager* focusManager)
        : mFocusManager(focusManager)
    {
        mTextureName = "sprite";

        SetSize(258.0f, 282.0f);
        SetSource(771.0f, 0.0f, 258.0f, 282.0f);

    }

    void GuiFrame::SetFrame(bool bForce)
    {
        m_isMove = true;
    }

    void GuiFrame::AnimationFinished(const std::string& name)
    {
        if (name == "frame-move")
        {
            m_isMove = false;
            LOG(LOG_CLASSIC_DEBUG, "finish animation frame");
        }
        else
        {    
            LOG(LOG_CLASSIC_DEBUG, "finish other animation frame");
        }
        
    }

    void GuiFrame::Click()
    {
        const float width = 256;
        const float height = 280;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = GetTransform();

        target.scale.x = scale;
        target.scale.y = scale;

        target.position.x += (-width / 2 * target.scale.x) + width / 2;
        target.position.y += (-height / 2 * target.scale.y) + height / 2;

        target.color.a = 0;
        GetAnimationManager().StartAnimation("card-zoom", time, this, target, Ease::EaseQuadInOut, true);
        //GetTimerManager()->SetTimer(mTimer, [this]() { m_transform.color.a = 255; }, this, time * 1);
    }

    void GuiFrame::Update()
    {
        
        std::vector<FocusComponent*> focusComponents = mFocusManager->GetAllFocusComponents();
        for (auto& focus : focusComponents)
        {
            if (focus->IsFocus())
            {
                Transform target = GetTransform();
                const float x = Math::Clamp(focus->GetPositionFocus().x, 130.0f, 898.0f);
                const float y = focus->GetPositionFocus().y;

                if (x == GetPosition().x && y == GetPosition().y)
                {
                    return;
                }

                if (m_isMove && !GetAnimation("frame-move").GetAnimationIsRun() && !GetAnimation("card-zoom").GetAnimationIsRun())
                {
                    if (GetAnimation("frame-move").GetAnimationIsRun())
                    {
                        LOG(LOG_CLASSIC_DEBUG, "call move frame");
                    }

                    target.position.x = x;
                    target.position.y = y;
                    GetAnimationManager().StartAnimation("frame-move", 0.15f, this, target, Ease::EaseQuadInOut, false);
                }
            }
        }

        Animatable::UpdateAnimation();
    }

} // namespace ClassicLauncher