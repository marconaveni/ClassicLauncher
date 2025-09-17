#include "Keyboard.h"

namespace ray
{
#include "raylib.h"
} // namespace ray


namespace ClassicLauncher::Keyboard
{
    bool IsPressed(int key)
    {
        return ray::IsKeyPressed(key);
    }

    bool IsDown(int key)
    {
        return ray::IsKeyDown(key);
    }

    bool IsReleased(int key)
    {
        return ray::IsKeyReleased(key);
    }

    bool IsUp(int key)
    {
        return ray::IsKeyUp(key);
    }


} // namespace ClassicLauncher::Keyboard
