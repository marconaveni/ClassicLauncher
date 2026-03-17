#ifndef GUI_BASE_H
#define GUI_BASE_H

#include "Entity/Entity.h"

namespace ClassicLauncher
{

    class GuiBase : public Entity
    {
    public:

        explicit GuiBase(const EntityContext& entityContext);
        virtual EntityType GetType() const override { return EntityType::GuiBaseClass; }
    };

} // namespace ClassicLauncher

#endif // GUI_BASE_H