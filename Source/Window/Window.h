#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>
#include "Data/Vector2.h"

namespace ClassicLauncher
{

    //struct Image;

    class Window
    {
    public:

        virtual void Init(int width, int height, const std::string& title) = 0;
        virtual bool ShouldClose() = 0;
        virtual void Close() = 0;

        virtual void SetState(unsigned int flags) = 0;
        virtual void ClearState(unsigned int flags) = 0;
        virtual bool IsState(unsigned int flag) = 0;
        virtual void SetSize(int width, int height) = 0;
        virtual void SetPosition(int x, int y) = 0;
        virtual void SetIcons(const std::vector<std::string>& pathIcons) = 0;
        virtual void SetExitKey(int key) = 0;
        virtual void SetTargetFPS(int fps) = 0;
    };

};  // namespace ClassicLauncher

#endif