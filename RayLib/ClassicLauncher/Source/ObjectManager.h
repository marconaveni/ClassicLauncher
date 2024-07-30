#pragma once


#include <memory>
#include <vector>
#include <memory>

#include "Card.h"
#include "Object.h"


class ObjectManager
{
    ObjectManager() = default;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<Object*> objectsGet;
    
public:
    static ObjectManager* GetInstance();
    std::vector<Object*>& GetAllObjects();
    void RegisterObject(const std::shared_ptr<Object>& object);
    void UnRegisterObject();
    void BeginPlay();
    void Tick() const;
    void Draw() const;
    void EndDraw() const;
    void EndPlay() const;
};


