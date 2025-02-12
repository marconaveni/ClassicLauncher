#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <string>
#include <vector>
#include "Entity.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{
    class Entity;
    class TimerManager;

    class EntityManager
    {
    private:

        std::vector<std::unique_ptr<Entity>> mEntities;
        std::vector<std::unique_ptr<Entity>> mTempEntities;
        std::vector<EntityType> mTypeCount;
        SpriteManager* mSpriteManagerReference;
        TimerManager* mTimerManagerReference;
        bool mPrepareNewOrdination = false;
        bool mHasNewEntity = false;
        void SetZOrder();
        void SetNewEntities();
        void SetNameId(Entity* entity, const std::string& name);

    public:

        EntityManager(SpriteManager* spriteManagerReference, TimerManager* timerManagerReference);
        ~EntityManager();

        template <typename T, typename... Args>
        T* CreateEntity(const std::string& name, Args&&... args)
        {
            auto entity = std::make_unique<T>(std::forward<Args>(args)...);
            SetNameId(entity.get(), name);
            mTempEntities.push_back(std::move(entity));
            return static_cast<T*>(mTempEntities.back().get());
        }

        template <typename T>
        std::vector<T*> GetEntitiesType(EntityType type)
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

        std::vector<std::unique_ptr<Entity>>& GetEntities() { return mEntities; }
        inline int GetEntitySize() { return static_cast<int>(mEntities.size() + mTempEntities.size()); }
        void SetVisibleAll(Entity* entity, bool bVisible);
        void SetZOrder(Entity* entity, int zOrder);
        void UpdateAll();
        void UpdatePositionAll();
        void Draw();
        void End();
        void ClearAllEntitys();

    private:

        void DrawEntity(Entity* entity);
        void DeleteEntitys(bool bIsDeleteEntities);
    };

}  // namespace ClassicLauncher

#endif  // ENTITYMANAGER_H