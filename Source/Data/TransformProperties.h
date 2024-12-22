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
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;
        float sourceX = 0;
        float sourceY = 0;
        float scaleWidth = 0;
        float scaleHeight = 0;
        float scale = 1;
        float rotation = 0;
        Color color = { 255, 255, 255, 255 };
    };

}  // namespace ClassicLauncher

#endif  // TRANSFORM_PROPERTIES_H