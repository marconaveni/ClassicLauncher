#ifndef RENDER_ENTITIES_H
#define RENDER_ENTITIES_H

#include <memory>

#include "Entity/Entity.h"

namespace ClassicLauncher
{

    class SpriteManager;
    class ConfigurationManager;

    class RenderEntities
    {
    public:

        explicit RenderEntities(SpriteManager* spriteManagerReference, ConfigurationManager* configManager);
        void DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities);

    private:

        void DrawEntity(Entity* entity);
        void DrawDebug(Entity* entity);
        void DrawStatistics(Entity* entity);
        void DrawThemeReference();
        ConfigurationManager* m_configManagerReference{nullptr};
        SpriteManager* m_spriteManagerReference{nullptr};
        float m_renderScale{1};
    };

} // namespace ClassicLauncher

#endif