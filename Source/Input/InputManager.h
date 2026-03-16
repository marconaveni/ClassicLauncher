#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <cstdint>

#include "Input/Gamepad.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace ClassicLauncher
{
    // clang-format off
    enum InputCategory
    {    
        MainBottom =        0x00000001,       
        MainCenter =        0x00000010,       
        MainTop =           0x00000100,       
        VideoFullscreen =   0x00001000,       
        Debug =             0x10000000       
    };
    // clang-format on

    enum InputName : std::uint8_t
    {
        Unknown = 0, // None
        DPadUp,      // DPad (Up)                Key(Up)
        DPadRight,   // DPad (Right)             Key(Right)
        DPadDown,    // DPad (Down)              Key(Down)
        DPadLeft,    // DPad (Left)              Key(Left)
        Triangle,    // PS (Triangle) XB (Y)     Key(F)
        Circle,      // PS (Circle)   XB (B)     Key(Backspace)
        Cross,       // PS (Cross)    XB (A)     Key(Enter)
        Square,      // PS (Square)   XB (X)     Key(A)
        L1,          // PS (L1)       XB (LB)    Key(Q)
        L2,          // PS (L2)       XB (LT)    Key(Z)
        R1,          // PS (R1)       XB (RB)    Key(E)
        R2,          // PS (R2)       XB (RT)    Key(C)
        Select,      // PS (Select)   XB (Back)  Key(Esc)
        PS,          // PS (Pslogo)   XB (Guide) Key(F1)
        Start,       // PS (Start)    XB (Start) Key(S)
        L3,          // PS (L3)       XB (LS)    Key(N)
        R3,          // PS (R3)       XB (RS)    Key(M)
    };

    struct InputMapper
    {
        int gamePad{0};
        int keyPad{0};
        float amoutDown{0.0f};
        InputName name{};
        bool isPress{false};
        bool isDown{false};
        bool isRelease{false};
        bool isUp{false};

        InputMapper(InputName name, int gamePad, int keyPad)
            : gamePad(gamePad)
            , keyPad(keyPad)
            , name(name)
        {
        }

        void CancelInput()
        {
            isPress = false;
            isDown = false;
            isRelease = false;
            isUp = false;
            amoutDown = 0;
        }
    };

    class InputManager
    {
    public:

        InputManager();
        ~InputManager();
        void UpdateInputState(float frameTime);

        static bool IsPress(InputName name, unsigned int category);
        static bool IsDown(InputName name, unsigned int category);
        static bool IsRelease(InputName name, unsigned int category);
        static bool IsUp(InputName name, unsigned int category);
        static void EnableInput();
        static void DisableInput();
        static void SetCategory(unsigned int category);
        static void RemoveCategory(unsigned int category);

    private:

        [[nodiscard]] bool CheckCategory(unsigned int category) const;

        int m_gamePadIdSelected{0};
        int m_amoutDown{0};
        bool m_disableInput{false};
        unsigned int m_category{0};

        InputMapper m_inputs[18]{InputMapper(Unknown, GamePad::Button::Unknown, Keyboard::Key::KeyNull),
                                 InputMapper(DPadUp, GamePad::Button::LeftFaceUp, Keyboard::Key::Up),
                                 InputMapper(DPadRight, GamePad::Button::LeftFaceRight, Keyboard::Key::Right),
                                 InputMapper(DPadDown, GamePad::Button::LeftFaceDown, Keyboard::Key::Down),
                                 InputMapper(DPadLeft, GamePad::Button::LeftFaceLeft, Keyboard::Key::Left),
                                 InputMapper(Triangle, GamePad::Button::RightFaceUp, Keyboard::Key::F),
                                 InputMapper(Circle, GamePad::Button::RightFaceRight, Keyboard::Key::Backspace),
                                 InputMapper(Cross, GamePad::Button::RightFaceDown, Keyboard::Key::Enter),
                                 InputMapper(Square, GamePad::Button::RightFaceLeft, Keyboard::Key::A),
                                 InputMapper(L1, GamePad::Button::LeftTrigger1, Keyboard::Key::Q),
                                 InputMapper(L2, GamePad::Button::LeftTrigger2, Keyboard::Key::Z),
                                 InputMapper(R1, GamePad::Button::RightTrigger1, Keyboard::Key::E),
                                 InputMapper(R2, GamePad::Button::RightTrigger2, Keyboard::Key::C),
                                 InputMapper(Select, GamePad::Button::MiddleLeft, Keyboard::Key::Escape),
                                 InputMapper(PS, GamePad::Button::Middle, Keyboard::Key::F1),
                                 InputMapper(Start, GamePad::Button::MiddleRight, Keyboard::Key::S),
                                 InputMapper(L3, GamePad::Button::LeftThumb, Keyboard::Key::N),
                                 InputMapper(R3, GamePad::Button::RightThumb, Keyboard::Key::M)};
    };

} // namespace ClassicLauncher

#endif // INPUT_MANAGER_H