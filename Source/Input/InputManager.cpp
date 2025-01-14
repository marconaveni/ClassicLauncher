#include "InputManager.h"

namespace ClassicLauncher
{

    static InputManager* sInstanceInputManager = nullptr;

    InputManager::InputManager()
        : mGamePadIdSelected(0), mAmoutDown(0), mDisableInput(false)
    {
        if (sInstanceInputManager == nullptr)
        {
            sInstanceInputManager = this;
        }
    }

    InputManager::~InputManager()
    {
        sInstanceInputManager = nullptr;
    }

    bool IsModifierKey()
    {
        return IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    }

    void InputManager::UpdateInputState()
    {
        for (auto& input : mInputs)
        {
            if (mDisableInput)
            {
                input.CancelInput();
                continue;
            }
            const bool bKeyModifier = IsModifierKey();
            const int maxAmount = 18;
            const int key = input.keyPad;
            const int gamePad = input.gamePad;
            input.bPress = (IsKeyPressed(key) || IsGamepadButtonPressed(mGamePadIdSelected, gamePad)) && !bKeyModifier && !mDisableInput;
            input.bDown = (IsKeyDown(key) || IsGamepadButtonDown(mGamePadIdSelected, gamePad)) && !bKeyModifier && !mDisableInput;
            input.bRelease = (IsKeyReleased(key) || IsGamepadButtonReleased(mGamePadIdSelected, gamePad)) && !bKeyModifier && !mDisableInput;
            input.bUp = (IsKeyUp(key) || IsGamepadButtonUp(mGamePadIdSelected, gamePad)) && !bKeyModifier && !mDisableInput;

            if (input.bDown)
            {
                input.amoutDown = Math::Clamp(input.amoutDown + 1, 0, maxAmount);
                input.bDown = (input.amoutDown == 1 || input.amoutDown == maxAmount);
            }
            else
            {
                input.amoutDown = 0;
            }
        }
    }

    bool InputManager::IsPress(InputName name)
    {
        if (!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bPress;
    }

    bool InputManager::IsDown(InputName name)
    {
        if (!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bDown;
    }

    bool InputManager::IsRelease(InputName name)
    {
        if (!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bRelease;
    }

    bool InputManager::IsUp(InputName name)
    {
        if (!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bUp;
    }

    void InputManager::EnableInput()
    {
        if (!sInstanceInputManager) return;
        sInstanceInputManager->mDisableInput = false;
    }

    void InputManager::DisableInput()
    {
        if (!sInstanceInputManager) return;
        sInstanceInputManager->mDisableInput = true;
        for (auto& input : sInstanceInputManager->mInputs)
        {
            input.CancelInput();
            continue;
        }
    }

}  // namespace ClassicLauncher
