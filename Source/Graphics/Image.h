#ifndef IMAGE
#define IMAGE

#include <filesystem>
#include "Data/Rectangle.h"

struct Image;

namespace ClassicLauncher
{

    struct Image
    {
        void* data;
        int width;
        int height;
        int mipmaps;
        int format;
    };



}  // namespace ClassicLauncher

#endif  // TEXTURE