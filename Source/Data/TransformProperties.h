#ifndef TRANSFORM_PROPERTIES_H
#define TRANSFORM_PROPERTIES_H

#include "Core.h"

namespace ClassicLauncher
{
    class TransformProperties 
    {

    public:

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
        int rootX = 0;
        int rootY = 0;
    };

}  // namespace ClassicLauncher

#endif  // TRANSFORM_PROPERTIES_H