#ifndef GUI_CANVAS
#define GUI_CANVAS

#include <vector>

#include "Entity/Entity.h"


namespace ClassicLauncher
{

    class GuiCanvas : public Entity
    {
    public:

        explicit GuiCanvas(const EntityContext& entityContext);
        ~GuiCanvas() = default;
        virtual EntityType GetType() const override { return EntityType::GuiCanvasClass; }
        virtual void AddChild(Entity* entity) override;
        virtual void Update() override;
        virtual void UpdateWorldTransform() override;

    private:

        std::vector<Entity*> m_entities{};
    };

} // namespace ClassicLauncher

#endif