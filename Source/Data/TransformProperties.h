#ifndef TRANSFORM_PROPERTIES_H
#define TRANSFORM_PROPERTIES_H

#include "Core.h"

namespace ClassicLauncher
{
    class TransformProperties
    {
    public:

        float rootX;
        float rootY;
        float rootScaleX;
        float rootScaleY;
        Vector2 offset;
        float x;
        float y;
        float width;
        float height;
        float sourceX;
        float sourceY;
        float scaleWidth;
        float scaleHeight;
        float scaleX;
        float scaleY;
        float rotation;
        Color color;

        TransformProperties()
            : rootX(0)
            , rootY(0)
            , rootScaleX(1)
            , rootScaleY(1)
            , offset()
            , x(0)
            , y(0)
            , width(0)
            , height(0)
            , sourceX(0)
            , sourceY(0)
            , scaleWidth(0)
            , scaleHeight(0)
            , scaleX(1)
            , scaleY(1)
            , rotation(0)
            , color({ 255, 255, 255, 255 })
        {
        }

        TransformProperties Multiply(const float m)
        {
            TransformProperties newTransf = *this;

            newTransf.rootX = rootX * m;
            newTransf.rootY = rootY * m;
            newTransf.x = (x + rootScaleX * offset.x) * m;
            newTransf.y = (y + rootScaleY * offset.y) * m;
            newTransf.width = width * m;
            newTransf.height = height * m;
            newTransf.sourceX = sourceX * m;
            newTransf.sourceY = sourceY * m;
            newTransf.scaleWidth = scaleWidth * m;
            newTransf.scaleHeight = scaleHeight * m;

            return newTransf;
        }
    };

}  // namespace ClassicLauncher

#endif  // TRANSFORM_PROPERTIES_H