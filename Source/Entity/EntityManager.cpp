#include "EntityManager.h"
#include <algorithm>  // std::sort
#include "raylib.h"

namespace ClassicLauncher
{
    EntityManager::EntityManager(SpriteManager* spriteManagerReference)
        : mSpriteManagerReference(spriteManagerReference)
    {
    }

    void EntityManager::SetVisibleAll(Entity* entity, bool bVisible)
    {
        for (auto& entity : entity->GetChilds())
        {
            entity->bVisible = bVisible;
        }
    }

    void EntityManager::SetZOrder(Entity* entity, int zOrder)
    {
        entity->SetZOrder(zOrder);
        std::sort(mEntities.begin(),
                  mEntities.end(),
                  [](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
                  {
                      return a->GetZOrder() < b->GetZOrder();
                  });
    }

    void EntityManager::UpdateAll()
    {
        for (auto& entity : mEntities)
        {
            entity->bToDraw = entity->bVisible;
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

    void EntityManager::DrawEntity(Entity* entity)
    {
        const Texture2D* texture = mSpriteManagerReference->GetTexture(entity->textureName);
        if (texture && entity->bToDraw)
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
            ::DrawTexturePro(*texture, source, dest, origin, rotation, color);
            entity->bToDraw = false;
            entity->bBringToFront = false;
        }
    }

    void EntityManager::Draw()
    {
        std::vector<Entity*> entitiesRenderFront;
        for (auto& entity : mEntities)
        {
            if (entity->bBringToFront)
            {
                entitiesRenderFront.emplace_back(entity.get());
                continue;
            }
            if (entity->bScissorMode)
            {
                const Rectangle scissorArea = entity->scissorArea;
                BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }
            DrawEntity(entity.get());
            if (entity->bScissorMode)
            {
                EndScissorMode();
            }
        }

        for (auto& entity : entitiesRenderFront)
        {
            if (entity->bScissorMode)
            {
                const Rectangle scissorArea = entity->scissorArea;
               // BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }
            DrawEntity(entity);
            if (entity->bScissorMode)
            {
               // EndScissorMode();
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
