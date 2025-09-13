#include "Mouse.h"

namespace ray
{
#include "raylib.h"
}  // namespace ray

namespace ClassicLauncher::Mouse
{

    bool IsPressed(int button)
    {
        return ray::IsMouseButtonPressed(button);
    }

    bool IsDown(int button)
    {
        return ray::IsMouseButtonDown(button);
    }
    
    bool IsReleased(int button)
    {
        return ray::IsMouseButtonReleased(button);
    }
    
    bool IsUp(int button)
    {
        return ray::IsMouseButtonUp(button);
    }


} // namespace ClassicLauncher::Mouse
