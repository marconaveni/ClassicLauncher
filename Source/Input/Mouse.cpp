#include "Mouse.h"

#include "Wrap.h"

namespace ClassicLauncher::Mouse
{

    bool IsPressed(int button)
    {
        return rlw::IsMouseButtonPressed(button);
    }

    bool IsDown(int button)
    {
        return rlw::IsMouseButtonDown(button);
    }

    bool IsReleased(int button)
    {
        return rlw::IsMouseButtonReleased(button);
    }

    bool IsUp(int button)
    {
        return rlw::IsMouseButtonUp(button);
    }


} // namespace ClassicLauncher::Mouse
