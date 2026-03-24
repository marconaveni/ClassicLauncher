#include "GamePadBackendSDL.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

#include "Helper.h"
#include "Input/Gamepad.h"


namespace ClassicLauncher::GamePad
{

    struct ButtonState
    {
        bool isPressed{false};
        bool isRelease{false};
    };

    struct AxisState
    {
        bool isPressed{false};
        bool isRelease{false};
        float axis{0.0f};
    };

    struct SDLGamePad
    {
        int numJoysticks{0};
        SDL_GameController* controller{nullptr};
        std::string name{"noname"};
        bool isReady{false};
        ButtonState button[18]{};
        AxisState axi[6]{};
    };


    int ClassicToSDLButton(uint8_t id)
    {
        switch (id)
        {
            case SDL_CONTROLLER_BUTTON_DPAD_UP: return LeftFaceUp;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return LeftFaceRight;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return LeftFaceDown;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return LeftFaceLeft;
            case SDL_CONTROLLER_BUTTON_Y: return RightFaceUp;
            case SDL_CONTROLLER_BUTTON_B: return RightFaceRight;
            case SDL_CONTROLLER_BUTTON_A: return RightFaceDown;
            case SDL_CONTROLLER_BUTTON_X: return RightFaceLeft;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return LeftTrigger1;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return RightTrigger1;
            case SDL_CONTROLLER_BUTTON_BACK: return MiddleLeft;
            case SDL_CONTROLLER_BUTTON_GUIDE: return Middle;
            case SDL_CONTROLLER_BUTTON_START: return MiddleRight;
            case SDL_CONTROLLER_BUTTON_LEFTSTICK: return LeftThumb;
            case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return RightThumb;
            default: break;
        }
        return -1;
    }

    int ClassicToSDLAxis(uint8_t axis)
    {
        switch (axis)
        {
            case SDL_CONTROLLER_AXIS_LEFTX: return LeftX; break;
            case SDL_CONTROLLER_AXIS_LEFTY: return LeftY; break;
            case SDL_CONTROLLER_AXIS_RIGHTX: return RightX; break;
            case SDL_CONTROLLER_AXIS_RIGHTY: return RightY; break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return LeftTrigger; break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return RightTrigger; break;
            default: break;
        }
        return -1;
    }

    static SDLGamePad s_gamepad;

    bool GamepadBackendSDL::Init()
    {

        if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
        {
            LOG(LogError, "Error starting up SDL: %s\n", SDL_GetError());
            return false;
        }

        SDL_GameControllerEventState(SDL_ENABLE);
        s_gamepad.numJoysticks = SDL_NumJoysticks();
        LOG(LogDebug, "Input devices detected: %d", s_gamepad.numJoysticks);

        for (int i = 0; i < s_gamepad.numJoysticks; i++)
        {
            if (!SDL_IsGameController(i))
            {
                LOG(LogError, "Device %d was not recognized as a Game Controller.: %s", i, SDL_JoystickNameForIndex(i));
                continue;
            }

            s_gamepad.controller = SDL_GameControllerOpen(i);
            if (!s_gamepad.controller)
            {
                LOG(LogError, "Error opening controller %d: %s", i, SDL_GetError());
                continue;
            }

            LOG(LogDebug, "Open Controller: %s", SDL_GameControllerName(s_gamepad.controller));
            break;
        }

        return true;
    }

    void GamePad::GamepadBackendSDL::Shutdown()
    {
        SDL_GameControllerClose(s_gamepad.controller);
        SDL_Quit();
    }

    void GamePad::GamepadBackendSDL::Update()
    {
        for (int i = 0; i < 18; i++)
        {
            s_gamepad.button[i].isRelease = false;
        }
        for (int i = 0; i < 6; i++)
        {
            s_gamepad.axi[i].isRelease = false;
        }


        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            const int buttonId = ClassicToSDLButton(event.jbutton.button);
            const int axisId = ClassicToSDLAxis(event.jaxis.axis);

            if (event.type == SDL_JOYDEVICEADDED)
            {
                s_gamepad.numJoysticks = SDL_NumJoysticks();
                LOG(LogDebug, "Input devices detected: %d", s_gamepad.numJoysticks);

                for (int i = 0; i < s_gamepad.numJoysticks; i++)
                {
                    if (!SDL_IsGameController(i))
                    {
                        LOG(LogError, "Device %d was not recognized as a Game Controller.: %s", i, SDL_JoystickNameForIndex(i));
                        continue;
                    }

                    s_gamepad.controller = SDL_GameControllerOpen(i);
                    if (!s_gamepad.controller)
                    {
                        LOG(LogError, "Error opening controller %d: %s", i, SDL_GetError());
                        continue;
                    }

                    LOG(LogDebug, "Open Controller: %s", SDL_GameControllerName(s_gamepad.controller));
                    break;
                }
            }
            if (event.type == SDL_JOYDEVICEREMOVED)
            {
                SDL_GameControllerClose(s_gamepad.controller);
                s_gamepad = {};
            }
            if (event.type == SDL_CONTROLLERBUTTONDOWN)
            {
                if (buttonId >= 0)
                {
                    s_gamepad.button[buttonId].isPressed = true;
                }
            }
            if (event.type == SDL_CONTROLLERBUTTONUP)
            {
                if (buttonId >= 0)
                {
                    s_gamepad.button[buttonId].isPressed = false;
                    s_gamepad.button[buttonId].isRelease = true;
                }
            }
            if (event.type == SDL_CONTROLLERAXISMOTION)
            {
                if (axisId >= 0)
                {
                    // SDL axis value range is -32768 to 32767, normalize to -1.0 to 1.0f range maintaining compatibility with raylib
                    const float value = event.jaxis.value / static_cast<float>(32767);
                    s_gamepad.axi[axisId].axis = value;
                    if (axisId == LeftTrigger && value < -0.1f || axisId == RightTrigger && value > 0.1f)
                    {
                        s_gamepad.axi[axisId].isPressed = true;
                    }
                    else if (s_gamepad.axi[axisId].isPressed == true)
                    {
                        s_gamepad.axi[axisId].isPressed = false;
                        s_gamepad.axi[axisId].isRelease = true;
                    }
                    const int buttonId = (axisId == LeftTrigger) ? LeftTrigger2 : RightTrigger2;
                    s_gamepad.button[buttonId].isPressed = s_gamepad.axi[axisId].isPressed;
                    s_gamepad.button[buttonId].isRelease = s_gamepad.axi[axisId].isRelease;
                }
            }
        }

        LOG(LogDebug, "A pressed %.1f", GetAxisMovement(0, Axis::LeftY));
        // LOG(LogDebug, "A pressed %s", TEXTBOOL( IsPressed(0, RightTrigger2) ));
        //  LOG(LogDebug, "A release %s", TEXTBOOL(s_gamepad.button[SDL_CONTROLLER_BUTTON_A].isRelease) );
    }

    bool GamePad::GamepadBackendSDL::IsAvailable(int gamepad) const
    {
        return false;
    }

    bool GamePad::GamepadBackendSDL::IsPressed(int gamepad, int button) const
    {

        return s_gamepad.button[button].isPressed;
    }

    bool GamePad::GamepadBackendSDL::IsDown(int gamepad, int button) const
    {

        return s_gamepad.button[button].isPressed;
    }

    bool GamePad::GamepadBackendSDL::IsReleased(int gamepad, int button) const
    {

        return s_gamepad.button[button].isRelease;
    }

    bool GamePad::GamepadBackendSDL::IsUp(int gamepad, int button) const
    {

        return !s_gamepad.button[button].isPressed;
    }

    float GamePad::GamepadBackendSDL::GetAxisMovement(int gamepad, int axis) const
    {
        return s_gamepad.axi[axis].axis;
    }

} // namespace ClassicLauncher::GamePad