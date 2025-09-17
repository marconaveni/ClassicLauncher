#ifndef MOUSE_H
#define MOUSE_H


namespace ClassicLauncher::Mouse
{
    // Mouse buttons
    enum Button
    {
        LEFT = 0,    // Mouse button left
        RIGHT = 1,   // Mouse button right
        MIDDLE = 2,  // Mouse button middle (pressed wheel)
        SIDE = 3,    // Mouse button side (advanced mouse device)
        EXTRA = 4,   // Mouse button extra (advanced mouse device)
        FORWARD = 5, // Mouse button forward (advanced mouse device)
        BACK = 6,    // Mouse button back (advanced mouse device)
    };

    bool IsPressed(int button);
    bool IsDown(int button);
    bool IsReleased(int button);
    bool IsUp(int button);

} // namespace ClassicLauncher::Mouse

#endif // MOUSE_H