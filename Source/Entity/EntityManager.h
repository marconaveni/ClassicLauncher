#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <memory>
#include <vector>
#include "Entity.h"
#include "Graphics/SpriteManager.h"


namespace ClassicLauncher
{

    class EntityManager
    {
    private:

        std::vector<std::shared_ptr<Entity>> mEntities;
        SpriteManager* mSpriteManagerReference;

    public:

        EntityManager(SpriteManager* spriteManagerReference);
        ~EntityManager() = default;

        template <typename T, typename... Args>
        std::shared_ptr<T> CreateEntity(Args&&... args)
        {
            static_assert(std::is_base_of<Entity, T>::value, "T deve herdar de Entity");
            auto entity = std::make_shared<T>(std::forward<Args>(args)...);
            mEntities.emplace_back(entity);
            return entity;
        }

        void UpdateAll();
        void UpdatePositionAll();
        void Draw();
        void End();
            
    };

}

#endif // ENTITYMANAGER_H