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
            entity->mVisible = bVisible;
        }
    }

    void EntityManager::SetZOrder(Entity* entity, int zOrder)
    {
        entity->SetZOrder(zOrder);
        std::sort(mEntities.begin(), mEntities.end(), [](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) { return a->GetZOrder() < b->GetZOrder(); });
    }

#ifdef _DEBUG
    static bool bEnable = false;
#endif

    void EntityManager::UpdateAll()
    {
#ifdef _DEBUG

        if (IsKeyReleased(KEY_FIVE))
        {
            bEnable = !bEnable;
        }
#endif
        for (auto& entity : mEntities)
        {
            entity->mToDraw = entity->mVisible;
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
        if (texture && entity->mToDraw)
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

            if (entity->mScissorMode)
            {
                const Rectangle scissorArea = entity->mScissorArea;
                BeginScissorMode(x + scissorArea.x, y + scissorArea.y, scissorArea.width, scissorArea.height);
            }

            entity->Draw();
            ::DrawTexturePro(*texture, source, dest, origin, properties.rotation, properties.color);

#ifdef _DEBUG

            const Rectangle recLine = { x, y, scaleWidth * properties.scale, scaleHeight * properties.scale };
            const Vector2 vec = Application::Get().GetRender()->GetMousePositionRender();
            if (CheckCollisionPointRec(vec, recLine) && bEnable)
            {
                ::DrawRectangleLinesEx(recLine, 2, Color::Red());
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    PRINT(TEXT("nameID: %s", entity->mNameId.c_str()), 5.0f);
                }
            }
            else if (bEnable)
            {
                ::DrawRectangleLinesEx(recLine, 1, Color::Cyan());
            }
#endif

            if (entity->mScissorMode)
            {
                EndScissorMode();
            }
            entity->mToDraw = false;
            entity->mBringToFront = false;
        }
    }

    void EntityManager::Draw()
    {
        std::vector<Entity*> entitiesRenderFront;
        for (auto& entity : mEntities)
        {
            if (entity->mBringToFront)
            {
                entitiesRenderFront.emplace_back(entity.get());
                continue;
            }
            DrawEntity(entity.get());
        }

        for (auto& entity : entitiesRenderFront)
        {
            DrawEntity(entity);
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
