#include "InputManager.h"

#include "Window/Window.h"
#include "Helper.h"

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
        return Keyboard::IsDown(Keyboard::IsDown(Keyboard::Key::LeftAlt) || //
                                Keyboard::IsDown(Keyboard::Key::RightAlt) ||
                                Keyboard::IsDown(Keyboard::Key::LeftControl) || // check if keys modifiers is pressed
                                Keyboard::IsDown(Keyboard::Key::RightControl));
    }

    void InputManager::UpdateInputState(float frameTime)
    {
        for (auto& input : m_inputs)
        {
            if (m_disableInput)
            {
                input.CancelInput();
                continue;
            }
            const bool isKeyModifier = IsModifierKey();
            const float maxAmount = 0.4f;
            const int key = input.keyPad;
            const int gamePad = input.gamePad;
            
            bool isAxisRelease = false;
            bool isAxisPress = false;
            
            if (input.directionAxis == InputMapper::Direction::Positive)
            {
                input.lastAxisValue = GamePad::GetAxisMovement(m_gamePadIdSelected, input.axis); 
                isAxisRelease = (input.lastAxisValue < 0.5f) && (input.isAxisDown);
                isAxisPress = (input.lastAxisValue > 0.5f) && (!input.isAxisDown);
                input.isAxisDown = (input.lastAxisValue > 0.5f);
            }
            else if (input.directionAxis == InputMapper::Direction::Negative)
            {
                input.lastAxisValue = GamePad::GetAxisMovement(m_gamePadIdSelected, input.axis); 
                isAxisRelease = (input.lastAxisValue > -0.5f) && (input.isAxisDown);
                isAxisPress = (input.lastAxisValue < -0.5f) && (!input.isAxisDown);
                input.isAxisDown = (input.lastAxisValue < -0.5f);
            }
            if (isAxisRelease)
            {
                LOG(LogTrace, "isAxisRelease");
            }
            if (isAxisPress)
            {
                LOG(LogTrace, "isAxisPress");
            }
            
            
            // clang-format off
            input.isPress = (Keyboard::IsPressed(key) || isAxisPress ||
                            GamePad::IsPressed(m_gamePadIdSelected, gamePad)) &&
                            !isKeyModifier && 
                            !m_disableInput;
            input.isDown = (Keyboard::IsDown(key) || input.isAxisDown ||
                            GamePad::IsDown(m_gamePadIdSelected, gamePad)) && 
                            !isKeyModifier &&
                            !m_disableInput;
            input.isRelease = (Keyboard::IsReleased(key) || isAxisRelease ||
                            GamePad::IsReleased(m_gamePadIdSelected, gamePad)) &&
                            !isKeyModifier && 
                            !m_disableInput;
            input.isUp = (Keyboard::IsUp(key) || 
                            GamePad::IsUp(m_gamePadIdSelected, gamePad)) && 
                            !isKeyModifier && 
                            !m_disableInput;

            // clang-format on
            if (input.isDown)
            {
                input.isDown = (input.amoutDown == 0 || input.amoutDown >= maxAmount);
                input.amoutDown += 0.016f * 60 * frameTime;
            }
            else
            {
                input.amoutDown = 0.0f;
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

        if ((m_category & MainTop) == (category & MainTop) && (category & MainTop) > 0)
        {
            value |= MainTop;
        }
        if ((m_category & MainCenter) == (category & MainCenter) && (category & MainCenter) > 0)
        {
            value |= MainCenter;
        }
        if ((m_category & MainBottom) == (category & MainBottom) && (category & MainBottom) > 0)
        {
            value |= MainBottom;
        }
        if ((m_category & VideoFullscreen) == (category & VideoFullscreen) && (category & VideoFullscreen) > 0)
        {
            value |= VideoFullscreen;
        }
        if ((m_category & Debug) == (category & Debug) && (category & Debug) > 0)
        {
            value |= Debug;
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

        if ((inputCategory & MainTop) != (category & MainTop) && (category & MainTop) > 0)
        {
            inputCategory |= MainTop;
        }
        if ((inputCategory & MainCenter) != (category & MainCenter) && (category & MainCenter) > 0)
        {
            inputCategory |= MainCenter;
        }
        if ((inputCategory & MainBottom) != (category & MainBottom) && (category & MainBottom) > 0)
        {
            inputCategory |= MainBottom;
        }
        if ((inputCategory & VideoFullscreen) != (category & VideoFullscreen) && (category & VideoFullscreen) > 0)
        {
            inputCategory |= VideoFullscreen;
        }
        if ((inputCategory & Debug) != (category & Debug) && (category & Debug) > 0)
        {
            inputCategory |= Debug;
        }
    }

    void InputManager::RemoveCategory(unsigned int category)
    {
        if (!s_instanceInputManager)
        {
            return;
        }

        unsigned int& inputCategory = s_instanceInputManager->m_category;

        if ((inputCategory & MainTop) > 0 && (category & MainTop) > 0)
        {
            inputCategory &= ~MainTop;
        }
        if ((inputCategory & MainCenter) > 0 && (category & MainCenter) > 0)
        {
            inputCategory &= ~MainCenter;
        }
        if ((inputCategory & MainBottom) > 0 && (category & MainBottom) > 0)
        {
            inputCategory &= ~MainBottom;
        }
        if ((inputCategory & VideoFullscreen) > 0 && (category & VideoFullscreen) > 0)
        {
            inputCategory &= ~VideoFullscreen;
        }
        if ((inputCategory & Debug) > 0 && (category & Debug) > 0)
        {
            inputCategory &= ~Debug;
        }
    }

} // namespace ClassicLauncher
