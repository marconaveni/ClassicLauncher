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
    class Window;

    class EntityManager
    {
    public:

        explicit EntityManager(SpriteManager* spriteManager, TimerManager* timerManager, FocusManager* focusManager, FontManager* fontManager, Window* window);
        ~EntityManager();

        template <typename T, typename... Args>
        T* CreateEntity(const std::string& name, Args&&... args)
        {
            EntityContext context{.entityManager = this,
                                  .timerManager = m_timerManagerReference,
                                  .spriteManager = m_spriteManagerReference,
                                  .focusManager = m_focusManagerReference,
                                  .fontManager = m_fontManagerReference,
                                  .window = m_windowReference};

            auto entity = std::make_unique<T>(context, std::forward<Args>(args)...);
            SetNameId(entity.get(), name);
            m_tempEntities.push_back(std::move(entity));
            return static_cast<T*>(m_tempEntities.back().get()); // .back last element vector
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
        static void SetVisibleAll(Entity* entity, bool isVisible);
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
        void DeleteEntities(bool isDeleteEntities);

        std::vector<std::unique_ptr<Entity>> m_entities{};
        std::vector<std::unique_ptr<Entity>> m_tempEntities{};
        std::vector<EntityType> m_typeCount{};

        SpriteManager* m_spriteManagerReference{nullptr};
        TimerManager* m_timerManagerReference{nullptr};
        FocusManager* m_focusManagerReference{nullptr};
        FontManager* m_fontManagerReference{nullptr};
        Window* m_windowReference{nullptr};

        bool m_markOrder{false};
        bool m_hasNewEntity{false};
        int m_counter{0};
    };

} // namespace ClassicLauncher

#endif // ENTITY_MANAGER_H