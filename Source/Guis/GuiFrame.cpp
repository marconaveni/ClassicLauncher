#include "GuiFrame.h"

#include "Components/FocusComponent.h"
#include "Components/FocusManager.h"
#include "Entity/EntityManager.h"
#include "Guis/GuiBase.h"
#include "Helper.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"

namespace ClassicLauncher
{

    GuiFrame::GuiFrame(Window* window)
        : Animatable(window)
    {
        m_textureName = "sprite";

        SetSize(258.0f, 282.0f);
        SetSource(771.0f, 0.0f, 258.0f, 282.0f);
        SetLimitArea(RectFloat{130.0f, 0.0f, 898.0f, 720.0f});
    }

    void GuiFrame::SetFrame(bool force)
    {
        m_isMove = true;
    }

    void GuiFrame::AnimationFinished(const std::string& name)
    {
        if (name == "frame-move")
        {
            m_isMove = false;
            FocusComponent* focus = GetFocusManager()->GetFocusComponent();
            if (focus && focus->GetFocusCategory() == FocusCategory::BUTTON_ICON)
            {
                m_frameMenu->SetOpacity(255);
                SetOpacity(0);
            }
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
    }

    void GuiFrame::SetLimitArea(RectFloat area)
    {
        // limit area frame move
        // ###################### window
        // #                    #
        // #   x#########width  #
        // #   #         #      #
        // #   #         #      #
        // #   y#########height #
        // #                    #
        // ######################
        m_limitAreaMove = area;
    }

    bool GuiFrame::IsFrameMove()
    {
        return GetAnimation("frame-move").GetAnimationIsRun();
    }

    void GuiFrame::UpdateFramePosition()
    {
        FocusComponent* focusComponent = GetFocusManager()->GetFocusComponent();
        if (!focusComponent)
        {
            return;
        }

        Transform target = GetTransform();

        const float left = ThemesManager::GetConfigurationThemes().offsetLeftFrame;
        const float right = ThemesManager::GetConfigurationThemes().offsetRightFrame;
        const float x = Math::Clamp(focusComponent->GetPositionFocus().x, m_limitAreaMove.x - left, m_limitAreaMove.width + right);
        const float y = Math::Clamp(focusComponent->GetPositionFocus().y, m_limitAreaMove.y, m_limitAreaMove.height);

        if (x == GetPosition().x && y == GetPosition().y)
        {
            return;
        }

        const bool isFrameMove = GetAnimation("frame-move").GetAnimationIsRun();
        const bool isCardZoom = GetAnimation("card-zoom").GetAnimationIsRun();

        if (!isFrameMove && !isCardZoom)
        {

            target.position.x = x;
            target.position.y = y;
            if (focusComponent->GetFocusCategory() == FocusCategory::CARD)
            {
                target.scale = 1.0f;
                target.color.a = 255.0f;
                SetOpacity(255);
                m_frameMenu->SetOpacity(0);
            }
            else
            {
                target.scale.x = 0.38f;
                target.scale.y = 0.25f;
            }

            GetAnimationManager().StartAnimation("frame-move", 0.15f, this, target, Ease::EaseQuadInOut, false);
        }
    }

    void GuiFrame::Update()
    {
        if (!m_frameMenu)
        {
            m_frameMenu = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
            m_frameMenu->SetSize({108, 82});
            m_frameMenu->SetSource(RectFloat{1030, 0, 108, 82});
            m_frameMenu->SetOffset(Vector2f{-6, -6});
            m_frameMenu->m_textureName = "sprite";
        }
        else
        {
            m_frameMenu->SetPosition(GetPosition());
        }


        UpdateFramePosition();
        Animatable::UpdateAnimation();
    }

} // namespace ClassicLauncher