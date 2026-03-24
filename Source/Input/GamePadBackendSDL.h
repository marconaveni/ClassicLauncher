#ifndef I_GAMEPAD_BACKEND_SDL_H
#define I_GAMEPAD_BACKEND_SDL_H

#include "Input/IGamePadBackend.h"


namespace ClassicLauncher::GamePad
{

    class GamepadBackendSDL final : public IGamepadBackend
    {
    public:

        virtual bool Init() override;
        virtual void Shutdown() override;
        virtual void Update() override;

        virtual bool IsAvailable(int gamepad) const override;
        virtual bool IsPressed(int gamepad, int button) const override;
        virtual bool IsDown(int gamepad, int button) const override;
        virtual bool IsReleased(int gamepad, int button) const override;
        virtual bool IsUp(int gamepad, int button) const override;
        virtual float GetAxisMovement(int gamepad, int axis) const override;

    };


} // namespace ClassicLauncher::GamePad

#endif