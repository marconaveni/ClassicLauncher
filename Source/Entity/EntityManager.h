#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "Entity.h"


namespace ClassicLauncher
{

    class TimerManager;
    class SpriteManager;
    class FocusManager;
    class FontManager;

    class EntityManager
    {
    public:

        EntityManager(SpriteManager* spriteManagerReference,
                      TimerManager* timerManagerReference,
                      FocusManager* focusManagerRef,
                      FontManager* fontManagerRef);
        ~EntityManager();

        template <typename T, typename... Args>
        T* CreateEntity(const std::string& name, Args&&... args)
        {
            auto entity = std::make_unique<T>(std::forward<Args>(args)...);
            entity->m_entityManagerReference = this;
            entity->m_timerManagerRef = m_timerManagerReference;
            entity->m_spriteManagerReference = m_spriteManagerReference;
            entity->m_focusManagerRef = m_focusManagerReference;
            entity->m_fontManagerRef = m_fontManagerReference;
            SetNameId(entity.get(), name);
            m_tempEntities.push_back(std::move(entity));
            return static_cast<T*>(m_tempEntities.back().get());  // .back last element vector
        }

        template <typename T>
        std::vector<T*> GetEntitiesType(EntityType type)
        {
            std::vector<T*> entities;
            for (const auto& entity : m_entities)
            {
                if (entity->GetType() == type)
                {
                    entities.emplace_back(static_cast<T*>(entity.get()));
                }
            }
            return entities;
        }

        std::vector<std::unique_ptr<Entity>>& GetEntities() { return m_entities; }
        int GetEntitySize() { return static_cast<int>(m_entities.size() + m_tempEntities.size()); }
        static void SetVisibleAll(Entity* entity, bool bVisible);
        void SetZOrder(Entity* entity, int zOrder);
        void UpdateAll();
        void UpdateWorldTransform();
        void End();
        void ClearAllEntities();
        void SetThemeValue();

    private:

        void SetZOrder();
        void SetNewEntities();
        void SetNameId(Entity* entity, const std::string& name);
        void DeleteEntities(bool bIsDeleteEntities);

        std::vector<std::unique_ptr<Entity>> m_entities;
        std::vector<std::unique_ptr<Entity>> m_tempEntities;
        std::vector<EntityType> m_typeCount;
        SpriteManager* m_spriteManagerReference;
        TimerManager* m_timerManagerReference;
        FocusManager* m_focusManagerReference;
        FontManager* m_fontManagerReference;
        bool m_markOrder = false;
        bool m_hasNewEntity = false;
        int m_counter = 0;
    };

} // namespace ClassicLauncher

#endif // ENTITY_MANAGER_H