#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Core.h"
#include "Entity.h"
#include "Graphics/SpriteManager.h"
#include "Utils/Timer.h"

namespace ClassicLauncher
{
    class Timer;
    class Entity;
    struct TimerHandling;

    class EntityManager
    {
    private:
        std::vector<std::unique_ptr<Entity>> mEntities;
        std::vector<EntityType> mTypeCount;
        SpriteManager* mSpriteManagerReference;
        std::unordered_map<int, std::unique_ptr<Timer>> mTimers;
        //std::vector<std::unique_ptr<Timer>> mTimers;
        bool mPreparedZOrder = false;
        void SetZOrder();
        void ValidTimerHandling(TimerHandling& timerHandling);

    public:

        EntityManager(SpriteManager* spriteManagerReference);
        ~EntityManager();

        template <typename T, typename... Args>
        T* CreateEntity(const std::string& name, Args&&... args)
        {
            auto entity = std::make_unique<T>(std::forward<Args>(args)...);
            const int counter = 0; //(int)std::count(mTypeCount.begin(), mTypeCount.end(), entity->GetType());
            entity->mNameId = std::to_string(counter) + "_" + name;
            entity->mId = static_cast<int>(mEntities.size());
            entity->mIdZOrder = static_cast<int>(mEntities.size());
            //mTypeCount.emplace_back(entity->GetType());
            mEntities.push_back(std::move(entity));
            return static_cast<T*>(mEntities.back().get());
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

        void SetTimer(TimerHandling& timerHandling, std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool bLooped = false);
        void SetVisibleAll(Entity* entity, bool bVisible);
        void SetZOrder(Entity* entity, int zOrder);
        void UpdateAll();
        void UpdatePositionAll();
        void Draw();
        void End();

    private:

        void DrawEntity(Entity* entity);
        void DeleteEntitys(bool bIsDeleteEntities);
    };

}  // namespace ClassicLauncher

#endif  // ENTITYMANAGER_H