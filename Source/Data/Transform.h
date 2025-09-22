#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Data/Color.h"
#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    class Transform
    {
    public:

        RectFloat position{};       // Posição local (relativa ao pai)
        Vector2f scale{1.0f, 1.0f}; // Escala local (relativa ao pai)
        float rotation{};           // Rotação local (relativa ao pai)
        Color color{255, 255, 255, 255};
        RectFloat source{}; // Área da textura a ser usada (isso está bom)
        Vector2f offset{};

    private:

        // friend class RenderEntities;
        // friend class GuiCanvas;
        // friend class GuiHorizontalBox;
        // friend class EntityGui;
        // friend class Animation;

    };


} // namespace ClassicLauncher

#endif // TRANSFORM_H