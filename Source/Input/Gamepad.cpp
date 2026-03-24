#include "Gamepad.h"

#include <memory>

#include "Input/GamePadBackendRay.h"
#include "Input/GamePadBackendSDL.h"
#include "Input/IGamePadBackend.h"
#include "Utils/Math.h"
#include "Wrap.h"



namespace ClassicLauncher::GamePad
{
    static std::unique_ptr<IGamepadBackend> s_backend;

    void Init(BackendType backend)
    {
        switch (backend)
        {
            case BackendType::SDL: s_backend = std::make_unique<GamepadBackendSDL>(); break;
            case BackendType::Raylib: s_backend = std::make_unique<GamepadBackendRaylib>(); break;
            default: break;
        }
        if (s_backend)
        {
            s_backend->Init();
        }
    }

    void Shutdown()
    {
        if (s_backend)
        {
            s_backend->Shutdown();
        }
    }

    void Update()
    {
        if (s_backend)
        {
            s_backend->Update();
        }
    }

    bool IsPressed(int gamepad, int button)
    {
        return s_backend ? s_backend->IsPressed(gamepad, button) : false;
    }

    bool IsDown(int gamepad, int button)
    {
        return s_backend ? s_backend->IsDown(gamepad, button) : false;
    }

    bool IsReleased(int gamepad, int button)
    {
        return s_backend ? s_backend->IsReleased(gamepad, button) : false;
    }

    bool IsUp(int gamepad, int button)
    {
        return s_backend ? s_backend->IsUp(gamepad, button) : false;
    }

    bool IsAvaliable(int gamepad)
    {
        return s_backend ? s_backend->IsAvailable(gamepad) : false;
    }

    float GetAxisMovement(int gamepad, int axis)
    {
        const float value = s_backend ? s_backend->GetAxisMovement(gamepad, axis) : 0.0f;
        return Math::Clamp(value, -1.0f, 1.0f);
    }


} // namespace ClassicLauncher::GamePad
