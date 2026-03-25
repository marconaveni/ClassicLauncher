#include "GamePadBackendSDL.h"

#ifdef SDL_GAMEPAD

    #define SDL_MAIN_HANDLED
    #include <SDL2/SDL.h>
    #include <string>

    #include "Helper.h"
    #include "Input/Gamepad.h"
    #include "Utils/Math.h"


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
        SDL_JoystickID id{-1};                   // typedef int32
        SDL_GameController* controller{nullptr}; // opace pointer
        std::string name{"noname"};
        bool isReady{false};
        ButtonState button[GamePadSpecs::MaxButtons]{};
        AxisState axi[GamePadSpecs::MaxAxis]{};
    };

    
    static SDLGamePad s_gamepad[GamePadSpecs::MaxGamePads];
    static int s_lastButtonPressed = 0; 

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

    bool GamepadBackendSDL::Init()
    {

        if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
        {
            LOG(LogError, "Error starting up SDL: %s\n", SDL_GetError());
            return false;
        }

        SDL_GameControllerEventState(SDL_ENABLE);

        const int numJoysticks = Math::Clamp(SDL_NumJoysticks(), 0, GamePadSpecs::MaxGamePads - 1);
        LOG(LogDebug, "Input devices detected: %d", numJoysticks);

        for (int i = 0; i < numJoysticks; i++)
        {
            if (!SDL_IsGameController(i))
            {
                LOG(LogError, "Device %d was not recognized as a Game Controller.: %s", i, SDL_JoystickNameForIndex(i));
                continue;
            }

            s_gamepad[i].controller = SDL_GameControllerOpen(i);
            s_gamepad[i].id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(s_gamepad[i].controller));
            s_gamepad[i].isReady = true;
            s_gamepad[i].name = SDL_GameControllerName(s_gamepad[i].controller);

            if (!s_gamepad[i].controller)
            {
                s_gamepad[i] = {};
                LOG(LogError, "Error opening controller %d: %s", i, SDL_GetError());
                continue;
            }

            LOG(LogDebug, "Open Controller[%d]: %s", s_gamepad[i].id, s_gamepad[i].name.c_str());
        }

        return true;
    }

    void GamePad::GamepadBackendSDL::Shutdown()
    {
        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            if (s_gamepad[i].controller && s_gamepad[i].isReady)
            {
                LOG(LogDebug, "Close Controller[%d]: %s", s_gamepad[i].id, s_gamepad[i].name.c_str());
                SDL_GameControllerClose(s_gamepad[i].controller);
            }
            s_gamepad[i] = {};
        }
        SDL_Quit();
    }

    void GamePad::GamepadBackendSDL::Update()
    {
        for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
        {
            if (!s_gamepad[i].isReady)
            {
                continue;
            }

            for (int j = 0; j < GamePadSpecs::MaxButtons; j++)
            {
                s_gamepad[i].button[j].isRelease = false;
            }
            for (int j = 0; j < GamePadSpecs::MaxAxis; j++)
            {
                s_gamepad[i].axi[j].isRelease = false;
            }
        }


        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            const int buttonId = ClassicToSDLButton(event.jbutton.button);
            const int axisId = ClassicToSDLAxis(event.jaxis.axis);


            if (event.type == SDL_JOYDEVICEADDED)
            {

                const int jdeviceId = event.jdevice.which; // Joystick device index

                for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
                {
                    if (jdeviceId == s_gamepad[i].id)
                    {
                        return;
                    }
                }

                int gamePadAvaliableSlots = -1;
                for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
                {
                    if (!s_gamepad[i].isReady && gamePadAvaliableSlots == -1)
                    {
                        gamePadAvaliableSlots = i;
                    }
                }

                if (gamePadAvaliableSlots == -1)
                {
                    return;
                }

                if (!s_gamepad[gamePadAvaliableSlots].isReady)
                {
                    s_gamepad[gamePadAvaliableSlots].controller = SDL_GameControllerOpen(jdeviceId);
                    s_gamepad[gamePadAvaliableSlots].id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(s_gamepad[gamePadAvaliableSlots].controller));

                    if (s_gamepad[gamePadAvaliableSlots].controller)
                    {
                        s_gamepad[gamePadAvaliableSlots].isReady = true;
                        s_gamepad[gamePadAvaliableSlots].name = SDL_GameControllerName(s_gamepad[gamePadAvaliableSlots].controller);
                        LOG(LogDebug, "Open Controller[%d]: %s", s_gamepad[gamePadAvaliableSlots].id, s_gamepad[gamePadAvaliableSlots].name.c_str());
                    }
                    else
                    {
                        s_gamepad[gamePadAvaliableSlots] = {};
                        LOG(LogError, "Error opening controller %d: %s", gamePadAvaliableSlots, SDL_GetError());
                    }
                }
            }
            else if (event.type == SDL_JOYDEVICEREMOVED)
            {

                const int jdeviceId = event.jdevice.which; // Joystick device index

                for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
                {
                    if (jdeviceId == s_gamepad[i].id)
                    {
                        SDL_GameControllerClose(s_gamepad[i].controller);
                        LOG(LogDebug, "Removed Controller[%d]: %s", s_gamepad[i].id, s_gamepad[i].name.c_str());
                        s_gamepad[i] = {};
                        break;
                    }
                }
            }
            else if (event.type == SDL_CONTROLLERBUTTONDOWN)
            {
                for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
                {
                    if (s_gamepad[i].id == event.jbutton.which)
                    {
                        if (buttonId >= 0)
                        {
                            s_gamepad[i].button[buttonId].isPressed = true;
                            s_lastButtonPressed = buttonId;
                            break;
                        }
                    }
                }
            }
            else if (event.type == SDL_CONTROLLERBUTTONUP)
            {
                for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
                {
                    if (s_gamepad[i].id == event.jbutton.which)
                    {
                        if (buttonId >= 0)
                        {
                            s_gamepad[i].button[buttonId].isPressed = false;
                            s_gamepad[i].button[buttonId].isRelease = true;
                            break;
                        }
                    }
                }
            }
            else if (event.type == SDL_CONTROLLERAXISMOTION)
            {

                for (int i = 0; i < GamePadSpecs::MaxGamePads; i++)
                {
                    if (s_gamepad[i].id == event.jbutton.which)
                    {

                        if (axisId >= 0)
                        {
                            // SDL axis value range is -32768 to 32767, normalize to -1.0 to 1.0f range maintaining compatibility with raylib
                            const float value = event.jaxis.value / static_cast<float>(32767);
                            s_gamepad[i].axi[axisId].axis = value;
                            if (axisId == LeftTrigger && value < -0.1f || axisId == RightTrigger && value > 0.1f)
                            {
                                s_gamepad[i].axi[axisId].isPressed = true;
                            }
                            else if (s_gamepad[i].axi[axisId].isPressed == true)
                            {
                                s_gamepad[i].axi[axisId].isPressed = false;
                                s_gamepad[i].axi[axisId].isRelease = true;
                            }
                            const int buttonId = (axisId == LeftTrigger) ? LeftTrigger2 : RightTrigger2;
                            s_gamepad[i].button[buttonId].isPressed = s_gamepad[i].axi[axisId].isPressed;
                            s_gamepad[i].button[buttonId].isRelease = s_gamepad[i].axi[axisId].isRelease;
                            break;
                        }
                    }
                }
            }
        }

        // LOG(LogDebug, "A pressed %.1f", GetAxisMovement(0, Axis::LeftY));
        //  LOG(LogDebug, "A pressed %s", TEXTBOOL( IsPressed(0, RightTrigger2) ));
    }

    bool GamePad::GamepadBackendSDL::IsAvailable(int gamepad) const
    {
        if (gamepad < GamePadSpecs::MaxGamePads)
        {
            return s_gamepad[gamepad].isReady;
        }
        return false;
    }

    bool GamePad::GamepadBackendSDL::IsPressed(int gamepad, int button) const
    {
        if (gamepad < GamePadSpecs::MaxGamePads)
        {
            return s_gamepad[gamepad].button[button].isPressed;
        }
        return false;
    }

    bool GamePad::GamepadBackendSDL::IsDown(int gamepad, int button) const
    {
        if (gamepad < GamePadSpecs::MaxGamePads)
        {
            return s_gamepad[gamepad].button[button].isPressed;
        }
        return false;
    }

    bool GamePad::GamepadBackendSDL::IsReleased(int gamepad, int button) const
    {
        if (gamepad < GamePadSpecs::MaxGamePads)
        {
            return s_gamepad[gamepad].button[button].isRelease;
        }
        return false;
    }

    bool GamePad::GamepadBackendSDL::IsUp(int gamepad, int button) const
    {
        if (gamepad < GamePadSpecs::MaxGamePads)
        {
            return !s_gamepad[gamepad].button[button].isPressed;
        }
        return false;
    }

    float GamePad::GamepadBackendSDL::GetAxisMovement(int gamepad, int axis) const
    {
        if (gamepad < GamePadSpecs::MaxGamePads && axis < GamePadSpecs::MaxAxis)
        {
            return s_gamepad[gamepad].axi[axis].axis;
        }
        return false;
    }

} // namespace ClassicLauncher::GamePad


#endif // SDL_GAMEPAD