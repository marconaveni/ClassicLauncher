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

    void Init()
    {
#ifdef SDL_GAMEPAD
        s_backend = std::make_unique<GamepadBackendSDL>();
#else
        s_backend = std::make_unique<GamepadBackendRaylib>();
#endif // SDL_GAMEPAD

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
        if (!s_backend)
        {
            return false;
        }

        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            if (s_backend->IsPressed(i, button))
            {
                return true;
            }
        }
        return false;
    }

    bool IsDown(int gamepad, int button)
    {
        if (!s_backend)
        {
            return false;
        }

        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            if (s_backend->IsDown(i, button))
            {
                return true;
            }
        }
        return false;
    }

    bool IsReleased(int gamepad, int button)
    {
        if (!s_backend)
        {
            return false;
        }

        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            if (s_backend->IsReleased(i, button))
            {
                return true;
            }
        }
        return false;
    }

    bool IsUp(int gamepad, int button)
    {
        if (!s_backend)
        {
            return false;
        }
        return s_backend ? s_backend->IsUp(gamepad, button) : false;
        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            if (s_backend->IsUp(i, button))
            {
                return true;
            }
        }
        return false;
    }

    bool IsAvaliable(int gamepad)
    {
        return s_backend ? s_backend->IsAvailable(gamepad) : false;
    }

    float GetAxisMovement(int gamepad, int axis)
    {
        if (!s_backend)
        {
            return 0.0f;
        }
        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            const float axisValue = s_backend ? s_backend->GetAxisMovement(i, axis) : 0.0f;
            if (axisValue > 0.0f || axisValue < 0.0f)
            {
                return axisValue;
            }
        }
        return 0.0f;
    }


} // namespace ClassicLauncher::GamePad
