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

    void EntityManager::UpdateAll()
    {
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
