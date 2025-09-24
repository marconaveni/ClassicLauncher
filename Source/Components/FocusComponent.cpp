#include "FocusComponent.h"

#include "Components/FocusManager.h"
#include "Entity/Entity.h"
#include "FocusManager.h"
#include "Helper.h"
#include "Themes/ThemesManager.h"

namespace ClassicLauncher
{
    FocusComponent::FocusComponent(FocusManager* focusManagerRef)
        : m_focusRef(focusManagerRef)
    {
        m_focusRef->AddFocus(this);
    }

    FocusComponent::~FocusComponent()
    {
        m_focusRef->RemoveFocus(this);
    }

    void FocusComponent::SetFocus()
    {
        m_focusRef->SetNewFocusComponent(this);
        m_isFocus = true;
        UpdateFocus();
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
