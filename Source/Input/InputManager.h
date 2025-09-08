#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "rl_wrap.h"


namespace ClassicLauncher
{
    // clang-format off
    enum InputCategory
    {    
        main =              0x00000001,       
        videoFullscreen =   0x00000010,       
        debug =             0x10000000       
    };
    // clang-format on

    enum InputName : std::uint8_t
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
        float amoutDown;
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

        int mGamePadIdSelected;
        int mAmoutDown;
        bool mDisableInput;
        unsigned int mCategory;

        // clang-format off
        InputMapper mInputs[18]{ 
            InputMapper(unknown, 0, rlw::KEY_NULL), 
            InputMapper(leftFaceUp, 1, rlw::KEY_UP), 
            InputMapper(leftFaceRight, 2, rlw::KEY_RIGHT), 
            InputMapper(leftFaceDown, 3, rlw::KEY_DOWN), 
            InputMapper(leftFaceLeft, 4, rlw::KEY_LEFT), 
            InputMapper(rightFaceUp, 5, rlw::KEY_F), 
            InputMapper(rightFaceRight, 6, rlw::KEY_BACKSPACE), 
            InputMapper(rightFaceDown, 7, rlw::KEY_ENTER), 
            InputMapper(rightFaceLeft, 8, rlw::KEY_A), 
            InputMapper(leftTriggerFront, 9, rlw::KEY_Q), 
            InputMapper(leftTriggerBack, 10, rlw::KEY_Z), 
            InputMapper(rightTriggerFront, 11, rlw::KEY_E), 
            InputMapper(rightTriggerBack, 12, rlw::KEY_C), 
            InputMapper(middleFaceLeft, 13, rlw::KEY_ESCAPE), 
            InputMapper(middleFaceCenter, 14, rlw::KEY_F1), 
            InputMapper(middleFaceRight, 15, rlw::KEY_S), 
            InputMapper(leftThumb, 16, rlw::KEY_N), 
            InputMapper(rightThumb, 17, rlw::KEY_M) 
            };
        // clang-format on

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

        [[nodiscard]]bool CheckCategory(unsigned int category) const;
    };

}  // namespace ClassicLauncher

#endif  // INPUT_MANAGER_H