#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "Entity/EntityGui.h"

namespace ClassicLauncher
{
    class GuiComponent : public EntityGui
    {
    public:

        GuiComponent();
        virtual ~GuiComponent() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
    };

}  // namespace ClassicLauncher

#endif  // GUI_COMPONENT_H