#include "FocusManager.h"
#include "Application.h"

namespace ClassicLauncher
{

    FocusManager::FocusManager()
    {
    }

    FocusManager::~FocusManager()
    {
    }

    void FocusManager::AddFocus(FocusComponent* focusComponent)
    {
        mFocusComponents.push_back(focusComponent);
    }

    void FocusManager::UpdateFocus(FocusComponent* focusComponent)
    {
        PRINT(TEXT("focus count %d", mFocusComponents.size()), 5.0f, "focusComp");
        for (auto& focus : mFocusComponents)
        {
            if (focus == focusComponent && !focus->mIsFocus)
            {
                focus->mIsFocus = true;
                focus->OnFocus();
            }
            else if (focus->mIsFocus)
            {
                focus->mIsFocus = false;
                focus->OnLostFocus();
            }
            focus->OnChangeFocus();
        }
    }

    void FocusManager::RemoveFocus(FocusComponent* focusComponent)
    {
        mFocusComponents.erase(std::remove_if(mFocusComponents.begin(),
                                              mFocusComponents.end(),
                                              [focusComponent](const FocusComponent* focus)
                                              {
                                                  return focus == focusComponent;  // Return true element
                                              }),
                               mFocusComponents.end());
    }

}  // namespace ClassicLauncher