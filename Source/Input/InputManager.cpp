#include "InputManager.h"

#include "Window/RayWindow.h"

namespace ClassicLauncher
{

    static InputManager* sInstanceInputManager = nullptr;

    InputManager::InputManager()
        : m_gamePadIdSelected(0), m_amoutDown(0), m_disableInput(false), m_category(0)
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
        for (auto& input : m_inputs)
        {
            if (m_disableInput)
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
                            GamePad::IsPressed(m_gamePadIdSelected, gamePad)) &&
                            !bKeyModifier && 
                            !m_disableInput;
            input.bDown = (Keyboard::IsDown(key) || 
                            GamePad::IsDown(m_gamePadIdSelected, gamePad)) && 
                            !bKeyModifier &&
                            !m_disableInput;
            input.bRelease = (Keyboard::IsReleased(key) || 
                            GamePad::IsReleased(m_gamePadIdSelected, gamePad)) &&
                            !bKeyModifier && 
                            !m_disableInput;
            input.bUp = (Keyboard::IsUp(key) || 
                            GamePad::IsUp(m_gamePadIdSelected, gamePad)) && 
                            !bKeyModifier && 
                            !m_disableInput;

            // clang-format on
            if (input.bDown)
            {
                input.bDown = (input.amoutDown == 0 || input.amoutDown >= maxAmount);
                input.amoutDown += 0.016f * 60 * RayWindow::GetFrameTime();
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
        return sInstanceInputManager->m_inputs[name].bPress && sInstanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsDown(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->m_inputs[name].bDown && sInstanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsRelease(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->m_inputs[name].bRelease && sInstanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsUp(InputName name, unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return false;
        }
        return sInstanceInputManager->m_inputs[name].bUp && sInstanceInputManager->CheckCategory(category);
    }

    void InputManager::EnableInput()
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        sInstanceInputManager->m_disableInput = false;
    }

    void InputManager::DisableInput()
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        sInstanceInputManager->m_disableInput = true;
        for (auto& input : sInstanceInputManager->m_inputs)
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

        if ((m_category & MAIN) == (category & MAIN) && (category & MAIN) > 0)
        {
            value |= MAIN;
        }
        if ((m_category & VIDEO_FULLSCREEN) == (category & VIDEO_FULLSCREEN) && (category & VIDEO_FULLSCREEN) > 0)
        {
            value |= VIDEO_FULLSCREEN;
        }
        if ((m_category & DEBUG) == (category & DEBUG) && (category & DEBUG) > 0)
        {
            value |= DEBUG;
        }

        return value != 0;
    }

    void InputManager::SetCategory(unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        unsigned int& pCategory = sInstanceInputManager->m_category;

        if ((pCategory & MAIN) != (category & MAIN) && (category & MAIN) > 0)
        {
            pCategory |= MAIN;
        }
        if ((pCategory & VIDEO_FULLSCREEN) != (category & VIDEO_FULLSCREEN) && (category & VIDEO_FULLSCREEN) > 0)
        {
            pCategory |= VIDEO_FULLSCREEN;
        }
        if ((pCategory & DEBUG) != (category & DEBUG) && (category & DEBUG) > 0)
        {
            pCategory |= DEBUG;
        }
    }

    void InputManager::RemoveCategory(unsigned int category)
    {
        if (!sInstanceInputManager)
        {
            return;
        }

        unsigned int& pCategory = sInstanceInputManager->m_category;

        if ((pCategory & MAIN) > 0 && (category & MAIN) > 0)
        {
            pCategory &= ~MAIN;
        }
        if ((pCategory & VIDEO_FULLSCREEN) > 0 && (category & VIDEO_FULLSCREEN) > 0)
        {
            pCategory &= ~VIDEO_FULLSCREEN;
        }
        if ((pCategory & DEBUG) > 0 && (category & DEBUG) > 0)
        {
            pCategory &= ~DEBUG;
        }
    }

} // namespace ClassicLauncher
