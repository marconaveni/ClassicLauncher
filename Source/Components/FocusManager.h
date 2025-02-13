#ifndef FOCUS_MANAGER_H
#define FOCUS_MANAGER_H

#include <vector>
#include "Components/FocusComponent.h"

namespace ClassicLauncher
{
    class FocusComponent;
    class Entity;

    class FocusManager
    {
    private:

        std::vector<FocusComponent*> mFocusComponents;

    public:

        FocusManager();
        ~FocusManager();
        void AddFocus(FocusComponent* focusComponent);
        void UpdateFocus(FocusComponent* focusComponent);
        void RemoveFocus(FocusComponent* focusComponent);
        std::vector<FocusComponent*>& GetAllFocusComponents() { return mFocusComponents; }
    };

}  // namespace ClassicLauncher

#endif