#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Core.h"
#include "Entity.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{
    class TimerBase;

    class EntityManager
    {
    private:

        std::vector<std::shared_ptr<Entity>> mEntities;
        std::vector<EntityType> mTypeCount;
        SpriteManager* mSpriteManagerReference;
        std::vector<std::shared_ptr<TimerBase>> mTimers;

    public:

        EntityManager(SpriteManager* spriteManagerReference);
        ~EntityManager() = default;

        template <typename T, typename... Args>
        std::shared_ptr<T> CreateEntity(const std::string& name, Args&&... args)
        {
            static_assert(std::is_base_of<Entity, T>::value, "T deve herdar de Entity");
            auto entity = std::make_shared<T>(std::forward<Args>(args)...);
            const int counter = std::count(mTypeCount.begin(), mTypeCount.end(), entity->GetType());
            entity->mNameId = std::to_string(counter) + "_" + name;
            entity->SetZOrder(mEntities.size());
            mEntities.emplace_back(entity);
            mTypeCount.emplace_back(entity->GetType());
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

        template <typename T>
        void SetTimer(Timer<T>*& timer, void (T::*callbackFunction)(), T* targetEntity, float delay, bool bLooped = false)
        {
            if (timer == nullptr)
            {
                static_assert(std::is_base_of<Entity, T>::value, "T deve herdar de Entity");
                TraceLog(LOG_ERROR,"Uma vez %d", targetEntity->GetType());
                auto newTimer = std::make_shared<Timer<T>>();
                mTimers.emplace_back(std::static_pointer_cast<TimerBase>(newTimer));
                timer = newTimer.get();
            }
            timer->SetTimer(callbackFunction, targetEntity, delay, bLooped);
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