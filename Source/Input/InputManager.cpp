#include "InputManager.h"

namespace ClassicLauncher
{

    static InputManager* sInstanceInputManager = nullptr;

    InputManager::InputManager()
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

    void InputManager::UpdateInputState()
    {
        for (auto& input : mInputs)
        {
            const int maxAmount = 18;
            const int key = input.keyPad;
            const int gamePad = input.gamePad;
            input.bPress = (IsKeyPressed(key) || IsGamepadButtonPressed(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT) && !mDisableInput;
            input.bDown = (IsKeyDown(key) || IsGamepadButtonDown(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT) && !mDisableInput;
            input.bRelease = (IsKeyReleased(key) || IsGamepadButtonReleased(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT) && !mDisableInput;
            input.bUp = (IsKeyUp(key) || IsGamepadButtonUp(mGamePadIdSelected, gamePad)) && !IsKeyDown(KEY_LEFT_ALT) && !mDisableInput;
            if (input.bDown)
            {
                input.amoutDown = Math::Clamp(input.amoutDown + 1, 0, maxAmount);
                input.bDown = input.amoutDown > 1 && input.amoutDown < maxAmount ? false : true;
            }
            else
            {
                input.amoutDown = 0;
            }
        }
    }

    InputManager& InputManager::Get()
    {
        return *sInstanceInputManager;
    }

    bool InputManager::IsPress(InputName name)
    {
        if(!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bPress;
    }

    bool InputManager::IsDown(InputName name)
    {
        if(!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bDown;
    }

    bool InputManager::IsRelease(InputName name)
    {
        if(!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bRelease;
    }

    bool InputManager::IsUp(InputName name)
    {
        if(!sInstanceInputManager) return false;
        return sInstanceInputManager->mInputs[name].bUp;
    }

    void InputManager::EnableInput()
    {
        if(!sInstanceInputManager) return ;
         sInstanceInputManager->mDisableInput = false;
    }

    void InputManager::DisableInput()
    {
        if(!sInstanceInputManager) return ;
         sInstanceInputManager->mDisableInput = true;
    }


}  // namespace ClassicLauncher
