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

        for (auto& entity : entities)
        {
            entity->Draw();
            Texture2D* texture = spriteManagerReference->GetSprite(entity->textureName);
            if(texture)
            {
                const int x = entity->x + entity->relativeX;
                const int y = entity->y + entity->relativeY;
                const int width = entity->width > 0 ? entity->width : texture->width;
                const int height = entity->height > 0 ? entity->height : texture->height;
                const float sourceX = entity->sourceX;
                const float sourceY = entity->sourceY;
                const float scaleWidth = entity->scaleWidth > 0 ? entity->scaleWidth : width ;
                const float scaleHeight = entity->scaleHeight > 0 ?  entity->scaleHeight : height;
                const float scale = entity->scale;


                const Rectangle source = { sourceX, sourceY, (float)width, (float)height };
                const Rectangle dest = { (float)x, (float)y, scaleWidth * scale, scaleHeight * scale};
                const Vector2 origin = { 0.0f, 0.0f };

                DrawTexturePro(*texture, source, dest, origin, 0, WHITE);
                //DrawTexture(*texture, x, y, WHITE);
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

