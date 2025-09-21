#include "FocusComponent.h"

#include "Components/FocusManager.h"
#include "Entity/Entity.h"
#include "Themes/ThemesManager.h"
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

        float m_renderScale = 1 ; //ThemesManager::GetScaleRenderer();

        Vector2f finalTransformVec{
            mEntity->m_worldTransform.position.x * mEntity->m_worldTransform.scale.x * m_renderScale,
            mEntity->m_worldTransform.position.y * mEntity->m_worldTransform.scale.y * m_renderScale
        };

        m_positionWorld = finalTransformVec;


    }


} // namespace ClassicLauncher
