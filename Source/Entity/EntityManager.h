#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <string>
#include <vector>
#include "Core.h"
#include "Entity.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{

    class EntityManager
    {
    private:

        std::vector<std::shared_ptr<Entity>> mEntities;
        std::vector<EntityType> typeCount;
        SpriteManager* mSpriteManagerReference;

    public:

        EntityManager(SpriteManager* spriteManagerReference);
        ~EntityManager() = default;

        template <typename T, typename... Args>
        std::shared_ptr<T> CreateEntity(const std::string& name, Args&&... args)
        {
            static_assert(std::is_base_of<Entity, T>::value, "T deve herdar de Entity");
            auto entity = std::make_shared<T>(std::forward<Args>(args)...);
            const int counter = std::count(typeCount.begin(), typeCount.end(), entity->GetType());
            entity->nameId = std::to_string(counter) + "_" + name;
            entity->SetZOrder(mEntities.size());
            mEntities.emplace_back(entity);
            typeCount.emplace_back(entity->GetType());
            return entity;
        }

        template <typename T>
        std::vector<T*> GetEntities(EntityType type)
        {
            std::vector<T*> entities;
            for (const auto& entity : mEntities)
            {
                if (entity->GetType() == type)
                {
                    entities.emplace_back(static_cast<T*>(entity.get()));
                }
            }
            return entities;
        }

        void SetVisibleAll(Entity* entity, bool bVisible);
        void SetZOrder(Entity* entity, int zOrder);
        void UpdateAll();
        void UpdatePositionAll();
        void Draw();
        void End();

    private:

        void DrawEntity(Entity* entity);
    };

}  // namespace ClassicLauncher

#endif  // ENTITYMANAGER_H