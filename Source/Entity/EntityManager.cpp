#include "EntityManager.h"
#include "raylib.h"

namespace ClassicLauncher
{
    EntityManager::EntityManager(SpriteManager* spriteManagerReference)
        : mSpriteManagerReference(spriteManagerReference)
    {
    }

    void EntityManager::UpdateAll()
    {
        for (auto& entity : mEntities)
        {
            entity->Update();
        }
    }

    void EntityManager::UpdatePositionAll()
    {
        for (auto& entity : mEntities)
        {
            entity->UpdatePosition();
        }
    }

    void EntityManager::Draw()
    {
        for (auto& entity : mEntities)
        {
            const Texture2D* texture = mSpriteManagerReference->GetTexture(entity->textureName);
            if (texture)
            {
                const int x = entity->x + entity->rootX;
                const int y = entity->y + entity->rootY;
                const int width = entity->width > 0 ? entity->width : texture->width;
                const int height = entity->height > 0 ? entity->height : texture->height;
                const float sourceX = entity->sourceX;
                const float sourceY = entity->sourceY;
                const float scaleWidth = entity->scaleWidth > 0 ? entity->scaleWidth : width;
                const float scaleHeight = entity->scaleHeight > 0 ? entity->scaleHeight : height;
                const float scale = entity->scale;
                const float rotation = entity->rotation;

                const Rectangle source = { sourceX, sourceY, (float)width, (float)height };
                const Vector2 origin = { (float)width / 2.0f, (float)height / 2.0f };
                const Rectangle dest = { (float)x + origin.x, (float)y + origin.y, scaleWidth * scale, scaleHeight * scale };
                const Color color = { entity->red, entity->green, entity->blue, entity->alpha };

                entity->Draw();
                DrawTexturePro(*texture, source, dest, origin, rotation, color);
            }
        }
    }

    void EntityManager::End()
    {
        for (auto& entity : mEntities)
        {
            entity->End();
        }
    }

}  // namespace ClassicLauncher
