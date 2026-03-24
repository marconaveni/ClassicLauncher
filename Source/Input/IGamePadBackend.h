#ifndef I_GAMEPAD_BACKEND_H
#define I_GAMEPAD_BACKEND_H

namespace ClassicLauncher::GamePad
{

    class IGamepadBackend
    {
    public:
        virtual ~IGamepadBackend() = default;

        virtual bool Init() = 0;
        virtual void Shutdown() = 0;
        virtual void Update() = 0;

        virtual bool IsAvailable(int gamepad) const = 0;
        virtual bool IsPressed(int gamepad, int button) const = 0;
        virtual bool IsDown(int gamepad, int button) const = 0;
        virtual bool IsReleased(int gamepad, int button) const = 0;
        virtual bool IsUp(int gamepad, int button) const = 0;
        virtual float GetAxisMovement(int gamepad, int axis) const = 0;

    };

}

#endif
