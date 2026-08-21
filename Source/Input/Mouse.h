#ifndef MOUSE_H
#define MOUSE_H


namespace ClassicLauncher::Mouse
{
    // Mouse buttons
    enum Button
    {
        Left = 0,    // Mouse button left
        Right = 1,   // Mouse button right
        Middle = 2,  // Mouse button middle (pressed wheel)
        Side = 3,    // Mouse button side (advanced mouse device)
        Extra = 4,   // Mouse button extra (advanced mouse device)
        Forward = 5, // Mouse button forward (advanced mouse device)
        Back = 6,    // Mouse button back (advanced mouse device)
    };

    bool IsPressed(int button);
    bool IsDown(int button);
    bool IsReleased(int button);
    bool IsUp(int button);

} // namespace ClassicLauncher::Mouse

#endif // MOUSE_H