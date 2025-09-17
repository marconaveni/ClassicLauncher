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
    private:

        friend class FocusManager;

        bool mIsFocus;
        FocusManager* m_focusRef;
        Entity* mEntity;

    public:

        FocusComponent(FocusManager* focusManagerRef, Entity* entity);
        ~FocusComponent();
        void SetFocus();
        bool GetFocus() { return mIsFocus; }
        Entity* GetEntity() { return mEntity; }
        virtual void OnFocus() = 0;
        virtual void OnLostFocus() = 0;
        virtual void OnChangeFocus() {}
    };

}  // namespace ClassicLauncher

#endif