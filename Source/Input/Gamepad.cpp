#include "Gamepad.h"

namespace ray
{
#include "raylib.h"
}  // namespace ray


namespace ClassicLauncher::GamePad
{
    bool IsPressed(int gamepad, int button)
    {
        return ray::IsGamepadButtonPressed(gamepad, button);
    }
    
    bool IsDown(int gamepad, int button)
    {
        return ray::IsGamepadButtonDown(gamepad, button);
    }
    
    bool IsReleased(int gamepad, int button)
    {
        return ray::IsGamepadButtonReleased(gamepad, button);
    }
    
    bool IsUp(int gamepad, int button)
    {
        return ray::IsGamepadButtonUp(gamepad, button);
    }

    bool IsAvaliable(int gamepad)
    {
        return ray::IsGamepadAvailable(gamepad);
    }


} // namespace ClassicLauncher::GamePad
