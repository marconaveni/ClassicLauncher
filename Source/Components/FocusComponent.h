#ifndef FOCUS_COMPONENT_H
#define FOCUS_COMPONENT_H

#include "Components/FocusManager.h"
#include "Entity/Entity.h"

namespace ClassicLauncher
{
    class Application;
    class Entity;
    class FocusManager;

    class FocusComponent
    {
    public:

        FocusComponent(FocusManager* focusManagerRef, Entity* entity);
        ~FocusComponent();
        void SetFocus();
        bool GetFocus() { return mIsFocus; }
        Entity* GetEntity() { return mEntity; }
        virtual void OnFocus() = 0;
        virtual void OnLostFocus() = 0;
        virtual void OnChangeFocus() {}
        void UpdateFocus();
        Vector2f GetPositionFocus() const { return m_positionWorld; }

    private:

        friend class FocusManager;
        friend class RenderEntities;

        bool mIsFocus = false;
        FocusManager* m_focusRef = nullptr;
        Entity* mEntity = nullptr;
        Vector2f m_positionWorld{};
    };

} // namespace ClassicLauncher

#endif