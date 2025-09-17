#include "InputManager.h"

#include "Window/RayWindow.h"

namespace ClassicLauncher
{

    static InputManager* sInstanceInputManager = nullptr;

    InputManager::InputManager()
        : mGamePadIdSelected(0), mAmoutDown(0), mDisableInput(false), mCategory(0)
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
        return Keyboard::IsDown(Keyboard::Key::LEFT_ALT) || // check if keys modifiers is pressed
               Keyboard::IsDown(Keyboard::Key::RIGHT_ALT) || Keyboard::IsDown(Keyboard::Key::LEFT_CONTROL) ||
               Keyboard::IsDown(Keyboard::Key::RIGHT_CONTROL);
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
            const float maxAmount = 0.4f;
            const int key = input.keyPad;
            const int gamePad = input.gamePad;

            // clang-format off

            input.bPress = (Keyboard::IsPressed(key) || 
                            GamePad::IsPressed(mGamePadIdSelected, gamePad)) &&
                            !bKeyModifier && 
                            !mDisableInput;
            input.bDown = (Keyboard::IsDown(key) || 
                            GamePad::IsDown(mGamePadIdSelected, gamePad)) && 
                            !bKeyModifier &&
                            !mDisableInput;
            input.bRelease = (Keyboard::IsReleased(key) || 
                            GamePad::IsReleased(mGamePadIdSelected, gamePad)) &&
                            !bKeyModifier && 
                            !mDisableInput;
            input.bUp = (Keyboard::IsUp(key) || 
                            GamePad::IsUp(mGamePadIdSelected, gamePad)) && 
                            !bKeyModifier && 
                            !mDisableInput;

            // clang-format on
            if (input.bDown)
            {
                input.bDown = (input.amoutDown == 0 || input.amoutDown >= maxAmount);
                input.amoutDown += RayWindow::GetFrameTime();
            }
            else
            {
                input.amoutDown = 0;
            }
        }
    }

    bool InputManager::IsPress(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->mInputs[name].bPress && sInstanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsDown(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->mInputs[name].bDown && sInstanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsRelease(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->mInputs[name].bRelease && sInstanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsUp(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->mInputs[name].bUp && sInstanceInputManager->CheckCategory(category);
    }

    void InputManager::EnableInput()
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        sInstanceInputManager->mDisableInput = false;
    }

    void InputManager::DisableInput()
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        sInstanceInputManager->mDisableInput = true;
        for (auto& input : sInstanceInputManager->mInputs)
        {
            input.CancelInput();
        }
    }

    bool InputManager::CheckCategory(unsigned int category) const
    {
        if (!sInstanceInputManager)
        {
            return false;
        }

        unsigned int value = 0;

        if ((mCategory & main) == (category & main) && (category & main) > 0)
        {
            value |= main;
        }
        if ((mCategory & videoFullscreen) == (category & videoFullscreen) && (category & videoFullscreen) > 0)
        {
            value |= videoFullscreen;
        }
        if ((mCategory & debug) == (category & debug) && (category & debug) > 0)
        {
            value |= debug;
        }

        return value != 0;
    }

    void InputManager::SetCategory(unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        unsigned int& pCategory = sInstanceInputManager->mCategory;

        if ((pCategory & main) != (category & main) && (category & main) > 0)
        {
            pCategory |= main;
        }
        if ((pCategory & videoFullscreen) != (category & videoFullscreen) && (category & videoFullscreen) > 0)
        {
            pCategory |= videoFullscreen;
        }
        if ((pCategory & debug) != (category & debug) && (category & debug) > 0)
        {
            pCategory |= debug;
        }
    }

    void InputManager::RemoveCategory(unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        unsigned int& pCategory = sInstanceInputManager->mCategory;

        if ((pCategory & main) > 0 && (category & main) > 0)
        {
            pCategory &= ~main;
        }
        if ((pCategory & videoFullscreen) > 0 && (category & videoFullscreen) > 0)
        {
            pCategory &= ~videoFullscreen;
        }
        if ((pCategory & debug) > 0 && (category & debug) > 0)
        {
            pCategory &= ~debug;
        }
    }

} // namespace ClassicLauncher
