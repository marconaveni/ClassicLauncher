#include "FocusManager.h"

#include "Components/FocusComponent.h"
#include "Helper.h"


namespace ClassicLauncher
{

    void FocusManager::AddFocus(FocusComponent* focusComponent)
    {
        mFocusComponents.push_back(focusComponent);
    }

    void FocusManager::SetNewFocusComponent(FocusComponent* focusComponent)
    {
        PRINT(TEXT("focus count %d", mFocusComponents.size()), 5.0f, "focusComp");
        for (auto& focus : mFocusComponents)
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
        for (auto& focus : mFocusComponents)
        {
            if (focus->m_isFocus)
            {
                focus->UpdateFocus();   
            }
        }
    }

    void FocusManager::RemoveFocus(FocusComponent* focusComponent)
    {
        mFocusComponents.erase(std::remove_if(mFocusComponents.begin(),
                                              mFocusComponents.end(),
                                              [focusComponent](const FocusComponent* focus)
                                              {
                                                  return focus == focusComponent; // Return true element
                                              }),
                               mFocusComponents.end());
    }

    FocusComponent* FocusManager::GetFocusComponent()
    {
        for (auto& focus : mFocusComponents)
        {
            if (focus->IsFocus())
            {
                return focus;
            }
        }
        return nullptr;
    }

} // namespace ClassicLauncher