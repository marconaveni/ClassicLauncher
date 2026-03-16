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
        MAIN_BOTTOM =        0x00000001,       
        MAIN_CENTER =        0x00000010,       
        MAIN_TOP =           0x00000100,       
        VIDEO_FULLSCREEN =   0x00001000,       
        DEBUG =              0x10000000       
    };
    // clang-format on

    enum InputName : std::uint8_t
    {
        unknown = 0,
        leftFaceUp,        // dpad (up)                                   key(up)
        leftFaceRight,     // dpad (right)                                key(right)
        leftFaceDown,      // dpad (down)                                 key(down)
        leftFaceLeft,      // dpad (left)                                 key(left)
        rightFaceUp,       // right right button PS (triangle) XB (Y)     key(f)
        rightFaceRight,    // right right button PS (circle) XB (B)       key(backspace)
        rightFaceDown,     // right right button PS (cross) XB (A)        key(enter)
        rightFaceLeft,     // right left button PS (square) XB (X)        key(a)
        leftTriggerFront,  // left trigger button PS (L1) XB (LB)         key(q)
        leftTriggerBack,   // left trigger button PS (L2) XB (LT)         key(z)
        rightTriggerFront, // right trigger button PS (R1) XB (RB)        key(e)
        rightTriggerBack,  // right trigger button PS (R2) XB (RT)        key(c)
        middleFaceLeft,    // center left button PS (select) XB (back)    key(esc)
        middleFaceCenter,  // center center button PS (pslogo) XB (guide) key(f1)
        middleFaceRight,   // center right button PS (start) XB (start)   key(s)
        leftThumb,         // left thumb button PS (L3) XB (LS)           key(n)
        rightThumb         // right thumb button PS (R3) XB (RS)          key(m)
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

    protected:

        int m_gamePadIdSelected{0};
        int m_amoutDown{0};
        bool m_disableInput{false};
        unsigned int m_category{0};

        InputMapper m_inputs[18]{InputMapper(unknown, 0, Keyboard::Key::KEYNULL),
                                 InputMapper(leftFaceUp, 1, Keyboard::Key::UP),
                                 InputMapper(leftFaceRight, 2, Keyboard::Key::RIGHT),
                                 InputMapper(leftFaceDown, 3, Keyboard::Key::DOWN),
                                 InputMapper(leftFaceLeft, 4, Keyboard::Key::LEFT),
                                 InputMapper(rightFaceUp, 5, Keyboard::Key::F),
                                 InputMapper(rightFaceRight, 6, Keyboard::Key::BACKSPACE),
                                 InputMapper(rightFaceDown, 7, Keyboard::Key::ENTER),
                                 InputMapper(rightFaceLeft, 8, Keyboard::Key::A),
                                 InputMapper(leftTriggerFront, 9, Keyboard::Key::Q),
                                 InputMapper(leftTriggerBack, 10, Keyboard::Key::Z),
                                 InputMapper(rightTriggerFront, 11, Keyboard::Key::E),
                                 InputMapper(rightTriggerBack, 12, Keyboard::Key::C),
                                 InputMapper(middleFaceLeft, 13, Keyboard::Key::ESCAPE),
                                 InputMapper(middleFaceCenter, 14, Keyboard::Key::F1),
                                 InputMapper(middleFaceRight, 15, Keyboard::Key::S),
                                 InputMapper(leftThumb, 16, Keyboard::Key::N),
                                 InputMapper(rightThumb, 17, Keyboard::Key::M)};


    public:

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
    };

} // namespace ClassicLauncher

#endif // INPUT_MANAGER_H