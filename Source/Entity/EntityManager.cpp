#include "EntityManager.h"

#include <algorithm> // std::sort
#include <format>

#include "Graphics/SpriteManager.h"
#include "Utils/TimerManager.h"
#include "Helper.h"

namespace ClassicLauncher
{

    EntityManager::EntityManager(SpriteManager* spriteManagerReference,
                                 TimerManager* timerManagerReference,
                                 FocusManager* focusManagerRef)
        : mSpriteManagerReference(spriteManagerReference)
        , mTimerManagerReference(timerManagerReference)
        , m_focusManagerReference(focusManagerRef)
    {
    }

    EntityManager::~EntityManager()
    {
        ClearAllEntities();
    }

    void EntityManager::SetNewEntities()
    {
        if (mTempEntities.empty())
        {
            return;
        }

        for (auto& entity : mTempEntities)
        {
            mEntities.push_back(std::move(entity));
        }
        mTempEntities.clear();
        m_markOrder = true;
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

        entity->mNameId = std::format("{}_{}", counter, name);
        entity->m_zOrder.insertionIndex = m_counter;
        m_counter++;
    }

    void EntityManager::SetVisibleAll(Entity* entity, bool bVisible)
    {
        for (auto& entity : entity->GetChildren())
        {
            entity->mVisible = bVisible;
        }
    }

    void EntityManager::SetZOrder()
    {
        if (!m_markOrder)
        {
            return;
        }

        
        // for (auto& e : mEntities)
        // {

        //     std::string name = std::format(
        //         "Entities insert {} zindex {} name {}",
        //            e->GetZOrder().insertionIndex, e->GetZOrder().id , e->mNameId
        //         );
        //     LOG(LOG_CLASSIC_WARNING, "%s", name.c_str());
        // }
        

        std::sort(mEntities.begin(),
                  mEntities.end(),
                  [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b)
                  {
                      if (a->GetZOrder().id != b->GetZOrder().id)
                      {
                          return a->GetZOrder().id < b->GetZOrder().id; // z menor desenha antes
                      }
                      return a->GetZOrder().insertionIndex < b->GetZOrder().insertionIndex;
                  });
        m_markOrder = false;


        // LOG(LOG_CLASSIC_ERROR, "#######################");
        // for (auto& e : mEntities)
        // {

        //     std::string name = std::format(
        //         "Entities insert {} zindex {} name {}",
        //            e->GetZOrder().insertionIndex, e->GetZOrder().id , e->mNameId
        //         );
        //     LOG(LOG_CLASSIC_WARNING, "%s", name.c_str());
        // }
    }

    void EntityManager::SetZOrder(Entity* entity, int zOrder)
    {
        entity->SetZOrder(zOrder);
        m_markOrder = true;
    }

    void EntityManager::UpdateAll()
    {
        SetNewEntities();

        for (auto& entity : mEntities)
        {
            entity->mToDraw = entity->mVisible;
            entity->Update();
        }
        UpdateWorldTransform();
    }

    void EntityManager::UpdateWorldTransform()
    {
        bool bIsDeleteEntities = false;
        for (auto& entity : mEntities)
        {
            entity->UpdateWorldTransform();
            bIsDeleteEntities = entity->mToDelete || bIsDeleteEntities;
        }
        DeleteEntities(bIsDeleteEntities);
        SetZOrder();
    }

    void EntityManager::End()
    {
        for (auto& entity : mEntities)
        {
            entity->End();
            entity->RemoveAllChildren();
        }
        ClearAllEntities();
    }

    void EntityManager::ClearAllEntities()
    {
        if (mEntities.empty())
        {
            return;
        }

        for (auto& entity : mEntities)
        {
            entity.reset();
            entity = nullptr;
        }
        mEntities.clear(); // Limpa o vetor
        mEntities.shrink_to_fit();
        m_counter = 0;
    }

    void EntityManager::SetThemeValue()
    {
        for (auto& entity : mTempEntities)
        {
            entity->SetThemeValue();
        }
        for (auto& entity : mEntities)
        {
            entity->SetThemeValue();
        }
    }

    void EntityManager::DeleteEntities(bool bIsDeleteEntities)
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
                                           return !entity; // Return true element
                                       }),
                        mEntities.end());

        mTimerManagerReference->ClearAllTimers();
        m_markOrder = true;
    }

} // namespace ClassicLauncher
