#ifndef ENTITY_H
#define ENTITY_H

#include <string>

namespace ClassicLauncher
{

    enum class EntityType
    {
        None = 0,
        GuiComponentClass = 1,
        GuiWindowClass = 2,
        GuiCardClass = 3,
        GuiGridClass = 4,
        GuiMiniCoverClass = 5,
    };

    class Entity
    {

        bool bToDelete = false;
        friend class EntityManager;

    public:

        Entity() = default;
        virtual ~Entity() = default;
        virtual EntityType GetType() const = 0;
        virtual void Update() {}
        virtual void UpdatePosition() {}
        virtual void Draw() {}
        virtual void End() {}
        virtual void SelfDelete() { bToDelete = true; }

        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        float sourceX = 0;
        float sourceY = 0;
        float scaleWidth = 0;
        float scaleHeight = 0;
        float scale = 1;
        float rotation = 0;
        unsigned char red = 255;    // Color red value
        unsigned char green = 255;  // Color green value
        unsigned char blue = 255;   // Color blue value
        unsigned char alpha = 255;  // Color alpha value
        std::string textureName;

        int rootX = 0;
        int rootY = 0;
    };

}  // namespace ClassicLauncher

#endif  // ENTITY_H