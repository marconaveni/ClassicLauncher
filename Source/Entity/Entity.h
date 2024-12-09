#ifndef ENTITY_H
#define ENTITY_H

#include <string>


namespace ClassicLauncher
{


    enum class EntityType
    {
        None = 0,
        GuiComponent = 1,
    };


    class Entity
    {

        bool bToDelete = false;
        friend class EntityManager;

    protected:


    public:

        Entity() = default;
        virtual ~Entity() = default;
        virtual EntityType GetType() const = 0;
        virtual void Update() {};
        virtual void Draw() {};
        virtual void End() {};
        virtual void SelfDelete() { bToDelete = true; };

        int x = 0;
        int y = 0;
        std::string textureName;
    protected:
        int relativeX = 0;
        int relativeY = 0;
    };
  

}

#endif // ENTITY_H