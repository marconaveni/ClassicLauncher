#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <memory>
#include <vector>
#include "Entity/Entity.h"

namespace ClassicLauncher
{
    class GuiComponent : public Entity
    {
    
    public:

        GuiComponent() = default;
        virtual ~GuiComponent() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
        virtual void Update() override;
    };

}  // namespace ClassicLauncher

#endif  // GUI_COMPONENT_H