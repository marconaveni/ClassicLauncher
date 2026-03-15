#include "InputManager.h"

#include "Window/WindowSystem.h"

namespace ClassicLauncher
{

    static InputManager* s_instanceInputManager = nullptr;

    InputManager::InputManager()
    {
        if (s_instanceInputManager == nullptr)
        {
            s_instanceInputManager = this;
        }
    }

    InputManager::~InputManager()
    {
        s_instanceInputManager = nullptr;
    }

    bool IsModifierKey()
    {
        return Keyboard::IsDown(Keyboard::Key::LEFT_ALT) || // check if keys modifiers is pressed
               Keyboard::IsDown(Keyboard::Key::RIGHT_ALT) || Keyboard::IsDown(Keyboard::Key::LEFT_CONTROL) || Keyboard::IsDown(Keyboard::Key::RIGHT_CONTROL);
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

            input.isPress = (Keyboard::IsPressed(key) || 
                            GamePad::IsPressed(m_gamePadIdSelected, gamePad)) &&
                            !bKeyModifier && 
                            !m_disableInput;
            input.isDown = (Keyboard::IsDown(key) || 
                            GamePad::IsDown(m_gamePadIdSelected, gamePad)) && 
                            !bKeyModifier &&
                            !m_disableInput;
            input.isRelease = (Keyboard::IsReleased(key) || 
                            GamePad::IsReleased(m_gamePadIdSelected, gamePad)) &&
                            !bKeyModifier && 
                            !m_disableInput;
            input.isUp = (Keyboard::IsUp(key) || 
                            GamePad::IsUp(m_gamePadIdSelected, gamePad)) && 
                            !bKeyModifier && 
                            !m_disableInput;

            // clang-format on
            if (input.isDown)
            {
                input.isDown = (input.amoutDown == 0 || input.amoutDown >= maxAmount);
                input.amoutDown += 0.016f * 60 * WindowSystem::Get().GetFrameTime();
            }
            else
            {
                input.amoutDown = 0;
            }
        }
    }

    bool InputManager::IsPress(InputName name, unsigned int category)
    {
        if (!s_instanceInputManager)
        {
            return false;
        }
        return s_instanceInputManager->m_inputs[name].isPress && s_instanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsDown(InputName name, unsigned int category)
    {
        if (!s_instanceInputManager)
        {
            return false;
        }
        return s_instanceInputManager->m_inputs[name].isDown && s_instanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsRelease(InputName name, unsigned int category)
    {
        if (!s_instanceInputManager)
        {
            return false;
        }
        return s_instanceInputManager->m_inputs[name].isRelease && s_instanceInputManager->CheckCategory(category);
    }

    bool InputManager::IsUp(InputName name, unsigned int category)
    {
        if (!s_instanceInputManager)
        {
            return false;
        }
        return s_instanceInputManager->m_inputs[name].isUp && s_instanceInputManager->CheckCategory(category);
    }

    void InputManager::EnableInput()
    {
        if (!s_instanceInputManager)
        {
            return;
        }

        s_instanceInputManager->m_disableInput = false;
    }

    void InputManager::DisableInput()
    {
        if (!s_instanceInputManager)
        {
            return;
        }

        s_instanceInputManager->m_disableInput = true;
        for (auto& input : s_instanceInputManager->m_inputs)
        {
            input.CancelInput();
        }
    }

    bool InputManager::CheckCategory(unsigned int category) const
    {
        if (!s_instanceInputManager)
        {
            return false;
        }

        unsigned int value = 0;

        if ((m_category & MAIN_TOP) == (category & MAIN_TOP) && (category & MAIN_TOP) > 0)
        {
            value |= MAIN_TOP;
        }
        if ((m_category & MAIN_CENTER) == (category & MAIN_CENTER) && (category & MAIN_CENTER) > 0)
        {
            value |= MAIN_CENTER;
        }
        if ((m_category & MAIN_BOTTOM) == (category & MAIN_BOTTOM) && (category & MAIN_BOTTOM) > 0)
        {
            value |= MAIN_BOTTOM;
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
        if (!s_instanceInputManager)
        {
            return;
        }

        unsigned int& inputCategory = s_instanceInputManager->m_category;

        if ((inputCategory & MAIN_TOP) != (category & MAIN_TOP) && (category & MAIN_TOP) > 0)
        {
            inputCategory |= MAIN_TOP;
        }
        if ((inputCategory & MAIN_CENTER) != (category & MAIN_CENTER) && (category & MAIN_CENTER) > 0)
        {
            inputCategory |= MAIN_CENTER;
        }
        if ((inputCategory & MAIN_BOTTOM) != (category & MAIN_BOTTOM) && (category & MAIN_BOTTOM) > 0)
        {
            inputCategory |= MAIN_BOTTOM;
        }
        if ((inputCategory & VIDEO_FULLSCREEN) != (category & VIDEO_FULLSCREEN) && (category & VIDEO_FULLSCREEN) > 0)
        {
            inputCategory |= VIDEO_FULLSCREEN;
        }
        if ((inputCategory & DEBUG) != (category & DEBUG) && (category & DEBUG) > 0)
        {
            inputCategory |= DEBUG;
        }
    }

    void InputManager::RemoveCategory(unsigned int category)
    {
        if (!s_instanceInputManager)
        {
            return;
        }

        unsigned int& inputCategory = s_instanceInputManager->m_category;

        if ((inputCategory & MAIN_TOP) > 0 && (category & MAIN_TOP) > 0)
        {
            inputCategory &= ~MAIN_TOP;
        }
        if ((inputCategory & MAIN_CENTER) > 0 && (category & MAIN_CENTER) > 0)
        {
            inputCategory &= ~MAIN_CENTER;
        }
        if ((inputCategory & MAIN_BOTTOM) > 0 && (category & MAIN_BOTTOM) > 0)
        {
            inputCategory &= ~MAIN_BOTTOM;
        }
        if ((inputCategory & VIDEO_FULLSCREEN) > 0 && (category & VIDEO_FULLSCREEN) > 0)
        {
            inputCategory &= ~VIDEO_FULLSCREEN;
        }
        if ((inputCategory & DEBUG) > 0 && (category & DEBUG) > 0)
        {
            inputCategory &= ~DEBUG;
        }
    }

} // namespace ClassicLauncher
