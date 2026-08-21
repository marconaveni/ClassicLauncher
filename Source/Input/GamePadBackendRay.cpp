#include "GamePadBackendRay.h"

#include "Helper.h"
#include "Wrap.h"

namespace ClassicLauncher::GamePad
{

    bool GamepadBackendRaylib::Init()
    {
        return true;
    }

    void GamePad::GamepadBackendRaylib::Shutdown()
    {
        LOG(LogTrace, "Raylib takes care of this");
    }

    void GamePad::GamepadBackendRaylib::Update()
    {
        LOG(LogTrace, "Raylib takes care of this");
    }

    bool GamePad::GamepadBackendRaylib::IsAvailable(int gamepad) const
    {
        return rlw::IsGamepadAvailable(gamepad);
    }

    bool GamePad::GamepadBackendRaylib::IsPressed(int gamepad, int button) const
    {
        return rlw::IsGamepadButtonPressed(gamepad, button);
    }

    bool GamePad::GamepadBackendRaylib::IsDown(int gamepad, int button) const
    {
        return rlw::IsGamepadButtonDown(gamepad, button);
    }

    bool GamePad::GamepadBackendRaylib::IsReleased(int gamepad, int button) const
    {
        return rlw::IsGamepadButtonReleased(gamepad, button);
    }

    bool GamePad::GamepadBackendRaylib::IsUp(int gamepad, int button) const
    {
        return rlw::IsGamepadButtonUp(gamepad, button);
    }

    float GamePad::GamepadBackendRaylib::GetAxisMovement(int gamepad, int axis) const
    {
        return rlw::GetGamepadAxisMovement(gamepad, axis);
    }

} // namespace ClassicLauncher::GamePad
