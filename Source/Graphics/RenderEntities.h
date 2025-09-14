#ifndef RENDER_ENTITIES_H
#define RENDER_ENTITIES_H

#include <memory>
#include "Entity/Entity.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{

    class RenderEntities
    {
    public:

        explicit RenderEntities(SpriteManager* spriteManagerReference);
        void DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities);
        void DrawEntity(Entity* entity);
        static void DrawDebug(Entity* entity);

    private:

        SpriteManager* mSpriteManagerReference;
    };

}  // namespace ClassicLauncher

#endif