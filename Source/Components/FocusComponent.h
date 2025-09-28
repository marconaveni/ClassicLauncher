#ifndef FOCUS_COMPONENT_H
#define FOCUS_COMPONENT_H

#include "Data/Vector2.h"

namespace ClassicLauncher
{
    class Transform;
    class FocusManager;

    class FocusComponent
    {
    public:

        explicit FocusComponent(FocusManager* focusManagerRef);
        virtual ~FocusComponent();
        void SetFocus();
        [[nodiscard]] bool IsFocus() const { return m_isFocus; }
        virtual void OnFocus() = 0;
        virtual void OnLostFocus() = 0;
        virtual void OnChangeFocus() {}
        void UpdateFocus();
        Vector2f GetPositionFocus() const { return m_positionWorld; }

    protected:

        virtual const Transform& OwnerWorldTransform() const = 0;

    private:

        friend class FocusManager;
        friend class RenderEntities;

        Transform* m_transformEntity = nullptr;
        bool m_isFocus = false;
        FocusManager* m_focusRef = nullptr;
        Vector2f m_positionWorld{};
    };

} // namespace ClassicLauncher

#endif