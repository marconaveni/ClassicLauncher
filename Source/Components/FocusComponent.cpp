#include "FocusComponent.h"

#include "Components/FocusManager.h"
#include "Entity/Entity.h"
#include "FocusManager.h"
#include "Themes/ThemesManager.h"

namespace ClassicLauncher
{
    FocusComponent::FocusComponent(FocusManager* focusManagerRef, Entity* entity)
        : m_focusRef(focusManagerRef), mEntity(entity)
    {
        m_focusRef->AddFocus(this);
    }

    FocusComponent::~FocusComponent()
    {
        m_focusRef->RemoveFocus(this);
    }

    void FocusComponent::SetFocus()
    {
        m_focusRef->SetFocus(this);
        mIsFocus = true;
        UpdateFocus();
    }

    void FocusComponent::UpdateFocus()
    {
        const Transform& transform = mEntity->m_worldTransform;

        m_positionWorld = {
            (transform.position.x + (transform.offset.x * transform.scale.x)),
            (transform.position.y + (transform.offset.y * transform.scale.y)),
        };
    }


} // namespace ClassicLauncher
