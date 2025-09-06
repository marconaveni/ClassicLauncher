#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "Entity/EntityGui.h"
#include "Entity/Entity.h"

namespace ClassicLauncher
{


    class GuiComponent : public EntityGui
    {
    public:

        GuiComponent();
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
    };

}  // namespace ClassicLauncher

#endif  // GUI_COMPONENT_H