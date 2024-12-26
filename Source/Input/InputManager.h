#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Core.h"

namespace ClassicLauncher
{
    struct InputMapper
    {
        bool leftFaceLeft = false;
        bool leftFaceRight = false;
        bool leftFaceUp = false;
        bool leftFaceDown = false;
        bool rightFaceLeft = false;
        bool rightFaceRight = false;
        bool rightFaceUp = false;
        bool rightFaceDown = false;
        bool middleFaceLeft = false;
        bool middleFaceRight = false;
    };

    enum PressedType
    {
        Pressed,   // Check if a gamepad button has been pressed once
        Down,      // Check if a gamepad button is being pressed
        Released,  // Check if a gamepad button has been released once
        Up         // Check if a gamepad button is NOT being pressed
    };

    class InputManager
    {
    private:

    public:

    
        static void EnableInput();
        static void DisableInput();
        static bool GetInputLeftFaceLeft(PressedType pressedType = PressedType::Released);
        static bool GetInputLeftFaceRight(PressedType pressedType = PressedType::Released);
        static bool GetInputLeftFaceUp(PressedType pressedType = PressedType::Released);
        static bool GetInputLeftFaceDown(PressedType pressedType = PressedType::Released);

        static bool GetInputRightFaceLeft(PressedType pressedType = PressedType::Released);
        static bool GetInputRightFaceRight(PressedType pressedType = PressedType::Released);
        static bool GetInputRightFaceUp(PressedType pressedType = PressedType::Released);
        static bool GetInputRightFaceDown(PressedType pressedType = PressedType::Released);

        static bool GetInputMiddleFaceLeft(PressedType pressedType = PressedType::Released);
        static bool GetInputMiddleFaceRight(PressedType pressedType = PressedType::Released);

        static bool GamepadButtonLeftThumb(PressedType pressedType = PressedType::Released);
        static bool GamepadButtonRightThumb(PressedType pressedType = PressedType::Released);

        static bool GetInput(PressedType pressedType, int key, int gamePad);
    };

}  // namespace ClassicLauncher

#endif  // INPUT_MANAGER_H