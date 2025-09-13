#ifndef GAMEPAD_H
#define GAMEPAD_H


namespace ClassicLauncher::GamePad
{
    // Gamepad buttons
    enum Button
    {
        UNKNOWN = 0,       // Unknown button, just for error checking
        LEFT_FACE_UP,      // Gamepad left DPAD up button
        LEFT_FACE_RIGHT,   // Gamepad left DPAD right button
        LEFT_FACE_DOWN,    // Gamepad left DPAD down button
        LEFT_FACE_LEFT,    // Gamepad left DPAD left button
        RIGHT_FACE_UP,     // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
        RIGHT_FACE_RIGHT,  // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
        RIGHT_FACE_DOWN,   // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
        RIGHT_FACE_LEFT,   // Gamepad right button left (i.e. PS3: Square, Xbox: X)
        LEFT_TRIGGER_1,    // Gamepad top/back trigger left (first), it could be a trailing button
        LEFT_TRIGGER_2,    // Gamepad top/back trigger left (second), it could be a trailing button
        RIGHT_TRIGGER_1,   // Gamepad top/back trigger right (first), it could be a trailing button
        RIGHT_TRIGGER_2,   // Gamepad top/back trigger right (second), it could be a trailing button
        MIDDLE_LEFT,       // Gamepad center buttons, left one (i.e. PS3: Select)
        MIDDLE,            // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
        MIDDLE_RIGHT,      // Gamepad center buttons, right one (i.e. PS3: Start)
        LEFT_THUMB,        // Gamepad joystick pressed button left
        RIGHT_THUMB        // Gamepad joystick pressed button right
    };


    // Gamepad axis
     enum Axis
    {
        LEFT_X = 0,        // Gamepad left stick X axis
        LEFT_Y = 1,        // Gamepad left stick Y axis
        RIGHT_X = 2,       // Gamepad right stick X axis
        RIGHT_Y = 3,       // Gamepad right stick Y axis
        LEFT_TRIGGER = 4,  // Gamepad back trigger left, pressure level: [1..-1]
        RIGHT_TRIGGER = 5  // Gamepad back trigger right, pressure level: [1..-1]
    };

    bool IsPressed(int gamepad, int button);
    bool IsDown(int gamepad, int button);
    bool IsReleased(int gamepad, int button);
    bool IsUp(int gamepad, int button);
    bool IsAvaliable(int gamepad);

}

#endif // GAMEPAD_H