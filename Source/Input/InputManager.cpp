#include "InputManager.h"

namespace ClassicLauncher
{

    int mGamePadIdSelected = 0;
    int sAmoutDown = 0;
    bool bDisableInput = false;

    void InputManager::EnableInput()
    {
        bDisableInput = false;
    }

    void InputManager::DisableInput()
    {
        bDisableInput = true;
    }

    bool InputManager::GetInputLeftFaceLeft(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_LEFT, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    }

    bool InputManager::GetInputLeftFaceRight(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_RIGHT, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    }

    bool InputManager::GetInputLeftFaceUp(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_UP, GAMEPAD_BUTTON_LEFT_FACE_UP);
    }

    bool InputManager::GetInputLeftFaceDown(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_DOWN, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    }

    bool InputManager::GetInputRightFaceLeft(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_G, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
    }

    bool InputManager::GetInputRightFaceRight(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_BACKSPACE, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    }

    bool InputManager::GetInputRightFaceUp(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_F, GAMEPAD_BUTTON_RIGHT_FACE_UP);
    }

    bool InputManager::GetInputRightFaceDown(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_ENTER, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    }

    bool InputManager::GetInputMiddleFaceLeft(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_O, GAMEPAD_BUTTON_MIDDLE_LEFT);
    }

    bool InputManager::GetInputMiddleFaceRight(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_P, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    }

    bool InputManager::GamepadButtonLeftThumb(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_Q, GAMEPAD_BUTTON_LEFT_THUMB);
    }

    bool InputManager::GamepadButtonRightThumb(PressedType pressedType)
    {
        return InputManager::GetInput(pressedType, KEY_E, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
    }

    bool InputManager::GetInput(PressedType pressedType, int key, int gamePad)
    {
        if (bDisableInput) return false;

        switch (pressedType)
        {
            case PressedType::Pressed:
                return (IsKeyPressed(key) || IsGamepadButtonPressed(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT);
            case PressedType::Down:
                return (IsKeyDown(key) || IsGamepadButtonDown(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT);
            case PressedType::Released:
                return (IsKeyReleased(key) || IsGamepadButtonReleased(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT);
            case PressedType::Up:
                return (IsKeyUp(key) || IsGamepadButtonUp(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT);
            default:
                return false;
        }
    }

}  // namespace ClassicLauncher
