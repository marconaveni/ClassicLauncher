#include "FocusManager.h"

#include "Components/FocusComponent.h"
#include "Helper.h"


namespace ClassicLauncher
{

    void FocusManager::AddFocus(FocusComponent* focusComponent)
    {
        m_focusComponents.push_back(focusComponent);
    }

    void FocusManager::SetNewFocusComponent(FocusComponent* focusComponent)
    {
        m_currentFocusComponent = nullptr;
        PRINT(TEXT("focus count %d", m_focusComponents.size()), 5.0f, "focusComp");
        for (auto& focus : m_focusComponents)
        {
            if (focus == focusComponent)
            {
                focus->m_isFocus = true;
                m_currentFocusComponent = focus;
                focus->OnFocus();
            }
            else if (focus->m_isFocus)
            {
                focus->m_isFocus = false;
                focus->OnLostFocus();
            }
            focus->OnChangeFocus();
        }
    }

    void FocusManager::Update()
    {
        for (auto& focus : m_focusComponents)
        {
            if (focus->m_isFocus)
            {
                focus->UpdateFocus();   
            }
        }
    }

    void FocusManager::RemoveFocus(FocusComponent* focusComponent)
    {
        m_focusComponents.erase(std::remove_if(m_focusComponents.begin(),
                                              m_focusComponents.end(),
                                              [focusComponent](const FocusComponent* focus)
                                              {
                                                  return focus == focusComponent; // Return true element
                                              }),
                               m_focusComponents.end());
    }


} // namespace ClassicLauncher