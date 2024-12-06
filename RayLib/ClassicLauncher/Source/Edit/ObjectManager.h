#pragma once

#include <memory>
#include <vector>

namespace ClassicLauncher
{

    class Object;

    class ObjectManager
    {
        // std::vector<std::shared_ptr<Object>> objects;
        /*    std::vector<Object*> objectsGet; */
    public:
        ObjectManager();
        ~ObjectManager();
        
        void RegisterObject(std::shared_ptr<Object> object);

        static ObjectManager& Get();
        void UnRegisterObject();
        void BeginPlay() const;
        void Tick() const;
        void Draw() const;
        void EndDraw() const;
        void EndPlay() const;
        /*     std::vector<Object*>& GetAllObjects();*/
    };

} // namespace ClassicLauncher
