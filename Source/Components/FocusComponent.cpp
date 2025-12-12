#include "FocusComponent.h"

#include "Components/FocusManager.h"
#include "Data/Transform.h"
#include "Themes/ThemesManager.h"

namespace ClassicLauncher
{
    FocusComponent::FocusComponent(FocusManager* focusManagerRef, FocusCategory focusCategory)
        : m_focusManagerReference(focusManagerRef), m_focusCategory(focusCategory)
    {
        m_focusManagerReference->AddFocus(this);
    }

    FocusComponent::~FocusComponent()
    {
        m_focusManagerReference->RemoveFocus(this);
    }

    void FocusComponent::SetFocus()
    {
        m_focusManagerReference->SetNewFocusComponent(this);
        m_isFocus = true;
        UpdateFocus();
    }

    void FocusComponent::RemoveFocus()
    {
        m_focusManagerReference->SetNewFocusComponent(nullptr);
        m_isFocus = false;
    }

    void FocusComponent::UpdateFocus()
    {
        const Transform& transform = OwnerWorldTransform();

        m_positionWorld = {
            (transform.position.x + (transform.offset.x * transform.scale.x)),
            (transform.position.y + (transform.offset.y * transform.scale.y)),
        };
    }


} // namespace ClassicLauncher
