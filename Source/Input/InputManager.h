#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Core.h"

namespace ClassicLauncher
{

    enum InputName
    {
        unknown = 0,
        leftFaceUp,         // dpad (up)                                   key(up)
        leftFaceRight,      // dpad (right)                                key(right)
        leftFaceDown,       // dpad (down)                                 key(down)
        leftFaceLeft,       // dpad (left)                                 key(left)
        rightFaceUp,        // right right button PS (triangle) XB (Y)     key(f)
        rightFaceRight,     // right right button PS (circle) XB (B)       key(backspace)
        rightFaceDown,      // right right button PS (cross) XB (A)        key(enter)
        rightFaceLeft,      // right left button PS (square) XB (X)        key(a)
        leftTriggerFront,   // left trigger button PS (L1) XB (LB)         key(q)
        leftTriggerBack,    // left trigger button PS (L2) XB (LT)         key(z)
        rightTriggerFront,  // right trigger button PS (R1) XB (RB)        key(e)
        rightTriggerBack,   // right trigger button PS (R2) XB (RT)        key(c)
        middleFaceLeft,     // center left button PS (select) XB (back)    key(esc)
        middleFaceCenter,   // center center button PS (pslogo) XB (guide) key(f1)
        middleFaceRight,    // center right button PS (start) XB (start)   key(s)
        leftThumb,          // left thumb button PS (L3) XB (LS)           key(n)
        rightThumb          // right thumb button PS (R3) XB (RS)          key(m)
    };

    struct InputMapper
    {
        int gamePad;
        int keyPad;
        int amoutDown;
        InputName name;
        bool bPress;
        bool bDown;
        bool bRelease;
        bool bUp;

        InputMapper(InputName name, int gamePad, int keyPad)
            : gamePad(gamePad), keyPad(keyPad), amoutDown(0), name(name), bPress(false), bDown(false), bRelease(false), bUp(false)
        {
        }

        void CancelInput()
        {
            bPress = false;
            bDown = false;
            bRelease = false;
            bUp = false;
            amoutDown = 0;
        }
    };

    class InputManager
    {
    public:

        InputManager();
        ~InputManager();
        void UpdateInputState();

    protected:

        int mGamePadIdSelected = 0;
        int mAmoutDown = 0;
        bool mDisableInput = false;

        // clang-format off
        InputMapper mInputs[18]{ 
            InputMapper(unknown, 0, KEY_NULL), 
            InputMapper(leftFaceUp, 1, KEY_UP), 
            InputMapper(leftFaceRight, 2, KEY_RIGHT), 
            InputMapper(leftFaceDown, 3, KEY_DOWN), 
            InputMapper(leftFaceLeft, 4, KEY_LEFT), 
            InputMapper(rightFaceUp, 5, KEY_F), 
            InputMapper(rightFaceRight, 6, KEY_BACKSPACE), 
            InputMapper(rightFaceDown, 7, KEY_ENTER), 
            InputMapper(rightFaceLeft, 8, KEY_A), 
            InputMapper(leftTriggerFront, 9, KEY_Q), 
            InputMapper(leftTriggerBack, 10, KEY_Z), 
            InputMapper(rightTriggerFront, 11, KEY_E), 
            InputMapper(rightTriggerBack, 12, KEY_C), 
            InputMapper(middleFaceLeft, 13, KEY_ESCAPE), 
            InputMapper(middleFaceCenter, 14, KEY_F1), 
            InputMapper(middleFaceRight, 15, KEY_S), 
            InputMapper(leftThumb, 16, KEY_N), 
            InputMapper(rightThumb, 17, KEY_M) 
            };
        // clang-format on

    public:

        static bool IsPress(InputName name);
        static bool IsDown(InputName name);
        static bool IsRelease(InputName name);
        static bool IsUp(InputName name);
        static void EnableInput();
        static void DisableInput();
    };

}  // namespace ClassicLauncher

#endif  // INPUT_MANAGER_H