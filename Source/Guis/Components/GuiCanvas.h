#ifndef GUI_CANVAS
#define GUI_CANVAS

#include "Entity/Entity.h"
#include <vector>

namespace ClassicLauncher
{
    class Entity;
    class Animatable;

    class GuiCanvas : public Entity
    {
    public:

        ~GuiCanvas() = default;
        virtual EntityType GetType() const override { return EntityType::GuiCanvasClass; }
        virtual void AddChild(Entity* entity) override;
        virtual void Update() override;   
        virtual void UpdateWorldTransform() override;
        // void SetOffset(Entity* entity, Vector2f position);
        // void SetOffset(Entity* entity, float x, float y);
        // [[nodiscard]] Vector2f GetOffset(Entity* entity) const;

    private:

        struct EntityPosition
        {
            Entity* entity = nullptr;
            Vector2f offset{};
        };

        std::vector<EntityPosition> m_entitiesPositions;
        
    };

} // namespace ClassicLauncher

#endif