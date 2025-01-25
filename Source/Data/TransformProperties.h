#ifndef TRANSFORM_PROPERTIES_H
#define TRANSFORM_PROPERTIES_H

#include "Core.h"

namespace ClassicLauncher
{
    class TransformProperties
    {
    public:

        float rootX = 0;
        float rootY = 0;
        float rootScaleX = 1;
        float rootScaleY = 1;
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;
        float sourceX = 0;
        float sourceY = 0;
        float scaleWidth = 0;
        float scaleHeight = 0;
        float scaleX = 1;
        float scaleY = 1;
        float rotation = 0;
        Color color = { 255, 255, 255, 255 };

        TransformProperties Multiply(const float m)
        {
            TransformProperties newTransf = *this;

            newTransf.rootX = rootX * m;
            newTransf.rootY = rootY * m;
            newTransf.x = x * m;
            newTransf.y = y * m;
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