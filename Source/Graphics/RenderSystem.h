#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory>
#include "Entity/Entity.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{

    class RenderSystem
    {
    private:

        SpriteManager* mSpriteManagerReference;

    public:

        RenderSystem(SpriteManager* spriteManagerReference);

        void DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities);
        void DrawEntity(Entity* entity);
        void DrawDebug(Entity* entity);
    };

}  // namespace ClassicLauncher

#endif