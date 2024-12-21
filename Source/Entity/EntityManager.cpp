#include "EntityManager.h"
#include <algorithm>  // std::sort


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


            const int x = entity->mProperties.x + entity->mProperties.rootX;
            const int y = entity->mProperties.y + entity->mProperties.rootY;
            const int width = entity->mProperties.width > 0 ? entity->mProperties.width : texture->width;
            const int height = entity->mProperties.height > 0 ? entity->mProperties.height : texture->height;
            const float sourceX = entity->mProperties.sourceX;
            const float sourceY = entity->mProperties.sourceY;
            const float scaleWidth = entity->mProperties.scaleWidth > 0 ? entity->mProperties.scaleWidth : width;
            const float scaleHeight = entity->mProperties.scaleHeight > 0 ? entity->mProperties.scaleHeight : height;
            const float scale = entity->mProperties.scale;
            const float rotation = entity->mProperties.rotation;

            const Rectangle source = { sourceX, sourceY, (float)width, (float)height };
            const Vector2 origin = { (float)width / 2.0f, (float)height / 2.0f };
            const Rectangle dest = { (float)x + origin.x, (float)y + origin.y, scaleWidth * scale, scaleHeight * scale };
            const Color color = { entity->mProperties.red, entity->mProperties.green, entity->mProperties.blue, entity->mProperties.alpha };


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
