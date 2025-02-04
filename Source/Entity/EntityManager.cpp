#include "EntityManager.h"
#include <algorithm>  // std::sort
#include "Application.h"

namespace ClassicLauncher
{

    EntityManager::EntityManager(SpriteManager* spriteManagerReference, TimerManager* timerManagerReference)
        : mSpriteManagerReference(spriteManagerReference), mTimerManagerReference(timerManagerReference)
    {
    }

    EntityManager::~EntityManager()
    {
        ClearAllEntitys();
    }

    void EntityManager::SetNewEntities()
    {
        if (mTempEntities.size() > 0)
        {
            for (auto& entity : mTempEntities)
            {
                mEntities.push_back(std::move(entity));
            }
            mTempEntities.clear();
            mPrepareNewOrdination = true;
        }
    }

    void EntityManager::SetNameId(Entity* entity, const std::string& name)
    {
        int counter = 0; 
        for (const auto& ent : mEntities)
        {
            if (ent->GetType() == entity->GetType())
            {
                counter++;
            }
        }
        for (const auto& ent : mTempEntities)
        {
            if (ent->GetType() == entity->GetType())
            {
                counter++;
            }
        }

        entity->mNameId = std::to_string(counter) + "_" + name;
        entity->mId = GetEntitySize();
        entity->mIdZOrder = GetEntitySize();
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
        if (!mPrepareNewOrdination)
        {
            return;
        }
        std::sort(
            mEntities.begin(), mEntities.end(), [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b) { return a->GetIdZOrder() < b->GetIdZOrder(); });
        mPrepareNewOrdination = false;
    }

    void EntityManager::SetZOrder(Entity* entity, int zOrder)
    {
        const int multiply = GetEntitySize() * zOrder;
        entity->SetZOrder(zOrder);
        entity->mIdZOrder = entity->mId + multiply;
        mPrepareNewOrdination = true;
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
        SetNewEntities();

        for (auto& entity : mEntities)
        {
            entity->mToDraw = entity->mVisible;
            entity->Update();
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
        }
    }

    void EntityManager::Draw()
    {
        for (auto& entity : mEntities)
        {
            DrawEntity(entity.get());
        }
    }

    void EntityManager::End()
    {
        for (auto& entity : mEntities)
        {
            entity->End();
            entity->RemoveAllChilds();
        }
        ClearAllEntitys();
    }

    void EntityManager::ClearAllEntitys()
    {
        if (mEntities.size() == 0)
        {
            return;
        }

        for (auto& entity : mEntities)
        {
            entity.reset();
            entity = nullptr;
        }
        mEntities.clear();  // Limpa o vetor
        mEntities.shrink_to_fit();
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

        mTimerManagerReference->ClearAllTimers();
        mPrepareNewOrdination = true;
    }

}  // namespace ClassicLauncher
