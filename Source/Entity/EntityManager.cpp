#include "EntityManager.h"
#include "raylib.h"


namespace ClassicLauncher
{
    EntityManager::EntityManager(SpriteManager* spriteManagerReference)
    : spriteManagerReference(spriteManagerReference)
    {
    }

    void EntityManager::UpdateAll()
    {
        for (auto& entity : entities)
        {
            entity->Update();
        }  
    }

    void EntityManager::Draw()
    {
        spriteManagerReference;
        for (auto& entity : entities)
        {
            entity->Draw();
            Texture2D* texture = spriteManagerReference->GetSprite(entity->textureName);
            if(texture)
            {
                const int x = entity->x + entity->relativeX;
                const int y = entity->y + entity->relativeY;
                DrawTexture(*texture, x, y, WHITE);
            }
        }  
    }

    void EntityManager::End()
    {
        for (auto& entity : entities)
        {
            entity->End();
        } 
    }

} // namespace ClassicLauncher

