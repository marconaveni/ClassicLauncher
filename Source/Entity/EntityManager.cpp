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
        : m_spriteManagerReference(spriteManagerReference)
        , m_timerManagerReference(timerManagerReference)
        , m_focusManagerReference(focusManagerRef)
    {
    }

    EntityManager::~EntityManager()
    {
        ClearAllEntities();
    }

    void EntityManager::SetNewEntities()
    {
        if (m_tempEntities.empty())
        {
            return;
        }

        for (auto& entity : m_tempEntities)
        {
            m_entities.push_back(std::move(entity));
        }
        m_tempEntities.clear();
        m_markOrder = true;
    }

    void EntityManager::SetNameId(Entity* entity, const std::string& name)
    {
        int counter = 0;
        for (const auto& ent : m_entities)
        {
            if (ent->GetType() == entity->GetType())
            {
                counter++;
            }
        }
        for (const auto& ent : m_tempEntities)
        {
            if (ent->GetType() == entity->GetType())
            {
                counter++;
            }
        }

        entity->m_nameId = std::format("{}_{}", counter, name);
        entity->m_zOrder.insertionIndex = m_counter;
        m_counter++;
    }

    void EntityManager::SetVisibleAll(Entity* entity, bool bVisible)
    {
        for (auto& entity : entity->GetChildren())
        {
            entity->m_isVisible = bVisible;
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
        

        std::sort(m_entities.begin(),
                  m_entities.end(),
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

        for (auto& entity : m_entities)
        {
            entity->m_isCanDraw = entity->m_isVisible;
            entity->Update();
        }
        UpdateWorldTransform();
    }

    void EntityManager::UpdateWorldTransform()
    {
        bool bIsDeleteEntities = false;
        for (auto& entity : m_entities)
        {
            entity->UpdateWorldTransform();
            bIsDeleteEntities = entity->m_isCanDelete || bIsDeleteEntities;
        }
        DeleteEntities(bIsDeleteEntities);
        SetZOrder();
    }

    void EntityManager::End()
    {
        for (auto& entity : m_entities)
        {
            entity->End();
            entity->RemoveAllChildren();
        }
        ClearAllEntities();
    }

    void EntityManager::ClearAllEntities()
    {
        if (m_entities.empty())
        {
            return;
        }

        for (auto& entity : m_entities)
        {
            entity.reset();
            entity = nullptr;
        }
        m_entities.clear(); // Limpa o vetor
        m_entities.shrink_to_fit();
        m_counter = 0;
    }

    void EntityManager::SetThemeValue()
    {
        for (auto& entity : m_tempEntities)
        {
            entity->SetThemeValue();
        }
        for (auto& entity : m_entities)
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

        for (auto& entity : m_entities)
        {
            if (entity->m_isCanDelete)
            {
                entity.reset();
            }
        }

        m_entities.erase(std::remove_if(m_entities.begin(),
                                       m_entities.end(),
                                       [](const std::unique_ptr<Entity>& entity)
                                       {
                                           return !entity; // Return true element
                                       }),
                        m_entities.end());

        m_timerManagerReference->ClearAllTimers(); 
        m_markOrder = true;
    }

} // namespace ClassicLauncher
