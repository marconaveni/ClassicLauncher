#ifndef RENDER_ENTITIES_H
#define RENDER_ENTITIES_H

#include <memory>

#include "Entity/Entity.h"

namespace ClassicLauncher
{

    class SpriteManager;

    class RenderEntities
    {
    public:

        explicit RenderEntities(SpriteManager* spriteManagerReference);
        void DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities);
        void DrawEntity(Entity* entity);
        void DrawDebug(Entity* entity);

    private:

        SpriteManager* m_spriteManagerReference = nullptr;
    };

} // namespace ClassicLauncher

#endif