#ifndef FOCUS_COMPONENT_H
#define FOCUS_COMPONENT_H

#include "Data/Vector2.h"

namespace ClassicLauncher
{
    class Transform;
    class FocusManager;

    enum class FocusCategory
    {
        CARD,
        BUTTON_ICON
    };

    class FocusComponent
    {
    public:

        explicit FocusComponent(FocusManager* focusManagerRef, FocusCategory focusCategory);
        virtual ~FocusComponent();
        void SetFocus();
        void RemoveFocus();
        [[nodiscard]] bool IsFocus() const { return m_isFocus; }
        virtual void OnFocus() = 0;
        virtual void OnLostFocus(FocusCategory previousFocusCategory) = 0;
        virtual void OnChangeFocus() {}
        void UpdateFocus();
        Vector2f GetPositionFocus() const { return m_positionWorld; }
        FocusCategory GetFocusCategory() const { return m_focusCategory; }

    protected:

        virtual const Transform& OwnerWorldTransform() const = 0;

    private:

        friend class FocusManager;
        friend class RenderEntities;

        bool m_isFocus{false};
        FocusManager* m_focusManagerReference{nullptr};
        Vector2f m_positionWorld{};
        FocusCategory m_focusCategory{};
    };

} // namespace ClassicLauncher

#endif