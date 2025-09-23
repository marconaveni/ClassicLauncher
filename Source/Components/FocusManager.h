#ifndef FOCUS_MANAGER_H
#define FOCUS_MANAGER_H

#include <vector>

namespace ClassicLauncher
{
    class FocusComponent;
    class Entity;

    class FocusManager
    {
    private:

        std::vector<FocusComponent*> mFocusComponents;
        FocusComponent* m_currentFocusComponent;

    public:

        FocusManager() = default;
        ~FocusManager() = default;
        void AddFocus(FocusComponent* focusComponent);
        void SetFocus(FocusComponent* focusComponent);
        void Update();
        void RemoveFocus(FocusComponent* focusComponent);
        std::vector<FocusComponent*>& GetAllFocusComponents() { return mFocusComponents; }
        FocusComponent* GetFocusComponent();
    };

} // namespace ClassicLauncher

#endif