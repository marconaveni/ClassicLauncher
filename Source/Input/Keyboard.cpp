#include "Keyboard.h"

#include "rl_wrap.h"


namespace ClassicLauncher::Keyboard
{
    bool IsPressed(int key)
    {
        return rlw::IsKeyPressed(key);
    }

    bool IsDown(int key)
    {
        return rlw::IsKeyDown(key);
    }

    bool IsReleased(int key)
    {
        return rlw::IsKeyReleased(key);
    }

    bool IsUp(int key)
    {
        return rlw::IsKeyUp(key);
    }


} // namespace ClassicLauncher::Keyboard
