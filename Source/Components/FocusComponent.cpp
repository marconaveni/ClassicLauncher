#include "FocusComponent.h"

#include "Components/FocusManager.h"
#include "Entity/Entity.h"
#include "FocusManager.h"

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
        m_focusRef->UpdateFocus(this);
        mIsFocus = true;
    }


} // namespace ClassicLauncher
