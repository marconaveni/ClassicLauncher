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
        int width = 0;
        int height = 0;
        float sourceX = 0;
        float sourceY = 0;
        float scaleWidth = 0;
        float scaleHeight = 0;
        float scale = 1;
        std::string textureName;

    protected:

        int relativeX = 0;
        int relativeY = 0;
    };
  

}

#endif // ENTITY_H