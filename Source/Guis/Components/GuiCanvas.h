#ifndef GUI_CANVAS
#define GUI_CANVAS

#include "Entity/Entity.h"
#include <vector>

namespace ClassicLauncher
{
    class Entity;

    class GuiCanvas : public Entity
    {
    public:

        ~GuiCanvas() = default;
        virtual EntityType GetType() const override { return EntityType::GuiCanvasClass; }
        virtual void AddChild(Entity* entity) override;
        virtual void Update() override;   
        virtual void UpdateWorldTransform() override;

    private:

        std::vector<Entity*> m_entities;
        
    };

} // namespace ClassicLauncher

#endif