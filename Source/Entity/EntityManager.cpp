#include "EntityManager.h"
#include <algorithm>  // std::sort
#include "Application.h"

namespace ClassicLauncher
{

    EntityManager::EntityManager(SpriteManager* spriteManagerReference)
        : mSpriteManagerReference(spriteManagerReference) {};

    EntityManager::~EntityManager()
    {
        for (auto& entity : mEntities)
        {
            if (entity != nullptr)
            {
                entity = nullptr;
            }
        }
        mEntities.clear();  // Limpa o vetor
    }

    void EntityManager::ValidTimerHandling(TimerHandling& timerHandling)
    {
        const int size = static_cast<int>(mTimers.size() - 1);
        if (timerHandling.id < 0 || timerHandling.id > size)
        {
            timerHandling.id = -1;
            return;
        }
    }

    void EntityManager::SetTimer(TimerHandling& timerHandling, std::function<void()> callbackFunction, Entity* targetEntity, float delay, bool bLooped)
    {
        ValidTimerHandling(timerHandling);

        if (timerHandling.id < 0)
        {
            std::unique_ptr<Timer> newTimer = std::make_unique<Timer>();
            timerHandling.id = mTimers.size();
            mTimers.insert(std::make_pair(timerHandling.id, std::move(newTimer)));
        }
        mTimers[timerHandling.id]->SetTimer(callbackFunction, targetEntity, delay, bLooped);
    }

    void EntityManager::SetVisibleAll(Entity* entity, bool bVisible)
    {
        for (auto& entity : entity->GetChilds())
        {
            entity->mVisible = bVisible;
        }
    }

    void EntityManager::SetZOrder()
    {
        if (!mPreparedZOrder)
        {
            return;
        }
        std::sort(
            mEntities.begin(), mEntities.end(), [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b) { return a->GetIdZOrder() < b->GetIdZOrder(); });
        mPreparedZOrder = false;
    }

    void EntityManager::SetZOrder(Entity* entity, int zOrder)
    {
        const int multiply = (int)mEntities.size() * zOrder;
        entity->SetZOrder(zOrder);
        entity->mIdZOrder = entity->mId + multiply;
        mPreparedZOrder = true;

        PRINT(TEXT("entity ZOrder %d", entity->GetZOrder()), 5.0f, "line344");
        PRINT(TEXT("entity IdZOrder %d", entity->GetIdZOrder()), 5.0f, "lindde344");
        PRINT(TEXT("entity Id %d", entity->mId), 5.0f, "line3d44");
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
            timer.second->Update();
        }
        UpdatePositionAll();
    }

    void EntityManager::UpdatePositionAll()
    {
        bool bIsDeleteEntities = false;
        for (auto& entity : mEntities)
        {
            entity->UpdatePosition();
            bIsDeleteEntities = entity->mToDelete || bIsDeleteEntities;
        }
        DeleteEntitys(bIsDeleteEntities);
        SetZOrder();
    }

    void EntityManager::DrawEntity(Entity* entity)
    {
        const Texture2D* texture = mSpriteManagerReference->GetTexture(entity->mTextureName);
        if (texture && entity->mToDraw)
        {
            TransformProperties properties = entity->mProperties;
            properties = properties.Multiply(Themes::GetScaleTexture());
            const float x = properties.x * properties.rootScaleX + properties.rootX;
            const float y = properties.y * properties.rootScaleY + properties.rootY;
            const float width = properties.width;
            const float height = properties.height;
            const float sourceX = properties.sourceX;
            const float sourceY = properties.sourceY;
            const float scaleWidth = properties.scaleWidth > 0.0f ? properties.scaleWidth : width;
            const float scaleHeight = properties.scaleHeight > 0.0f ? properties.scaleHeight : height;

            const Rectangle source = { sourceX, sourceY, width, height };
            const Vector2 scale = { (scaleWidth * properties.scaleX * properties.rootScaleX), (scaleHeight * properties.scaleY * properties.rootScaleY) };
            const Rectangle dest = { x, y, scale.x, scale.y };

            if (entity->mScissorMode)
            {
                const Rectangle scissorArea = entity->mScissorArea;
                BeginScissorMode(x + scissorArea.x, y + scissorArea.y, scissorArea.width, scissorArea.height);
            }

            entity->Draw();
            ::DrawTexturePro(*texture, source, dest, Vector2{ 0, 0 }, properties.rotation, properties.color);

#ifdef _DEBUG

            const Rectangle& RectangleDrawArea = dest;  //{ x, y, scale.x, scale.y };
            const Vector2 vec = Application::Get().GetRender()->GetMousePositionRender();
            if (CheckCollisionPointRec(vec, RectangleDrawArea) && bEnable)
            {
                ::DrawRectangleLinesEx(RectangleDrawArea, 2, Color::Red());
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    PRINT(TEXT("nameID: %s", entity->mNameId.c_str()), 5.0f);
                }
            }
            else if (bEnable)
            {
                ::DrawRectangleLinesEx(RectangleDrawArea, 1, Color::Cyan());
            }
            if (entity->mScissorMode && bEnable)
            {
                const Rectangle scissorArea = entity->mScissorArea;
                const Color tint = Color(255, 0, 0, 55);
                DrawRectangle(x + scissorArea.x, y + scissorArea.y, scissorArea.width, scissorArea.height, tint);
            }
#endif  // _DEBUG

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
            if (entity != nullptr)
            {
                entity->End();
                entity->RemoveAllChilds();
            }
        }
    }

    void EntityManager::DeleteEntitys(bool bIsDeleteEntities)
    {
        if (!bIsDeleteEntities)
        {
            return;
        }

        for (auto& entity : mEntities)
        {
            if (entity->mToDelete)
            {
                entity.reset();
            }
        }

        mEntities.erase(std::remove_if(mEntities.begin(),
                                       mEntities.end(),
                                       [](const std::unique_ptr<Entity>& entity)
                                       {
                                           return !entity;  // Return true element
                                       }),
                        mEntities.end());

        for (auto& timer : mTimers)
        {
            timer.second.reset();
        }
        mTimers.clear();
        mPreparedZOrder = true;
    }

}  // namespace ClassicLauncher
