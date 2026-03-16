#include "Gamepad.h"

#include "Wrap.h"


namespace ClassicLauncher::GamePad
{
    bool IsPressed(int gamepad, int button)
    {
        return rlw::IsGamepadButtonPressed(gamepad, button);
    }

    bool IsDown(int gamepad, int button)
    {
        return rlw::IsGamepadButtonDown(gamepad, button);
    }

    bool IsReleased(int gamepad, int button)
    {
        return rlw::IsGamepadButtonReleased(gamepad, button);
    }

    bool IsUp(int gamepad, int button)
    {
        return rlw::IsGamepadButtonUp(gamepad, button);
    }

    bool IsAvaliable(int gamepad)
    {
        return rlw::IsGamepadAvailable(gamepad);
    }

    float GetAxisMovement(int gamepad, int axis)
    {
        return rlw::GetGamepadAxisMovement(gamepad, axis);
    }


} // namespace ClassicLauncher::GamePad
