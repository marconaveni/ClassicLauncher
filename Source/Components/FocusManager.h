#ifndef FOCUS_MANAGER_H
#define FOCUS_MANAGER_H

#include <vector>

namespace ClassicLauncher
{
    class FocusComponent;

    class FocusManager
    {
    public:

        FocusManager() = default;
        ~FocusManager() = default;
        void AddFocus(FocusComponent* focusComponent);
        void SetNewFocusComponent(FocusComponent* focusComponent);
        void Update();
        void RemoveFocus(FocusComponent* focusComponent);
        std::vector<FocusComponent*>& GetAllFocusComponents() { return m_focusComponents; }
        FocusComponent* GetFocusComponent();

    private:

        std::vector<FocusComponent*> m_focusComponents{};
        FocusComponent* m_currentFocusComponent{nullptr};
    };

} // namespace ClassicLauncher

#endif