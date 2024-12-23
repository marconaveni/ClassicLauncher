#include "EntityManager.h"
#include <algorithm>  // std::sort
#include "Application.h"

namespace ClassicLauncher
{
    EntityManager::EntityManager(SpriteManager* spriteManagerReference)
        : mSpriteManagerReference(spriteManagerReference) {};


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
        for (auto& timer : mTimers)
        {
            timer->Update();
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
        const Texture2D* texture = mSpriteManagerReference->GetTexture(entity->mTextureName);
        if (texture && entity->bToDraw)
        {
            TransformProperties& properties = entity->mProperties;

            const float x = properties.x + properties.rootX;
            const float y = properties.y + properties.rootY;
            const float width = properties.width > 0.0f ? properties.width : texture->width;
            const float height = properties.height > 0.0f ? properties.height : texture->height;
            const float sourceX = properties.sourceX;
            const float sourceY = properties.sourceY;
            const float scaleWidth = properties.scaleWidth > 0.0f ? properties.scaleWidth : width;
            const float scaleHeight = properties.scaleHeight > 0.0f ? properties.scaleHeight : height;


            const Rectangle source = { sourceX, sourceY, width, height };
            const Vector2 origin = { width / 2.0f, height / 2.0f };
            const Rectangle dest = { x + origin.x, y + origin.y, scaleWidth * properties.scale, scaleHeight * properties.scale };

            entity->Draw();
            ::DrawTexturePro(*texture, source, dest, origin, properties.rotation, properties.color);

            entity->bToDraw = false;
            entity->bBringToFront = false;

#ifdef _DEBUG

            const Rectangle recLine = { x, y, scaleWidth * properties.scale, scaleHeight * properties.scale };
            const Vector2 vec = Application::Get().GetRender()->GetMousePositionRender();
            if (CheckCollisionPointRec(vec, recLine))
            {
                //::DrawRectangleLinesEx(recLine, 2, Color::Red());
            }
            else
            {
                //::DrawRectangleLinesEx(recLine, 1, Color::Cyan());
            }
#endif
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
                const Rectangle scissorArea = entity->mScissorArea;
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
                const Rectangle scissorArea = entity->mScissorArea;
                BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }
            DrawEntity(entity);
            if (entity->bScissorMode)
            {
                EndScissorMode();
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
