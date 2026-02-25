#ifndef RL_WRAP_H
#define RL_WRAP_H

// Wrapper minimalista para isolar a raylib.
// Incluir este header no projeto no lugar de <raylib.h>.
// A implementação (rl_wrap.cpp) inclui <raylib.h> e chama as funções nativas.
//
// Escopo: apenas funções observadas em uso no projeto.
// Namespace: rlw


#include "Data/Color.h"
#include "Data/Rectangle.h"
#include "Data/Vector2.h"
#include "Graphics/Font.h"
#include "Graphics/Image.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/Texture.h"

namespace rlw
{
    // --- Logging / Config  ---
    void SetTraceLogCallback(void (*callback)(int logLevel, const char* text, va_list args));
    void SetTraceLogLevel(int logLevel);


    // --- Drawing 2D Functions ---
    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(ClassicLauncher::Color color);


    // Scissor Functions
    void BeginScissorMode(int x, int y, int width, int height);
    void EndScissorMode();

    // Rectangles Functions
    void DrawRectangle(int x, int y, int width, int height, ClassicLauncher::Color color);
    void DrawRectangleLinesEx(ClassicLauncher::RectFloat rec, float lineThick, ClassicLauncher::Color color);

    // TextureMode Functions
    void BeginTextureMode(const ClassicLauncher::RenderTexture& target);
    void EndTextureMode();


    // Textures Functions
    void DrawTexture(const ClassicLauncher::Texture& texture, int posX, int posY, ClassicLauncher::Color tint);
    void DrawTexturePro(const ClassicLauncher::Texture& texture,
                        ClassicLauncher::RectFloat src,
                        ClassicLauncher::RectFloat dst,
                        ClassicLauncher::Vector2f origin,
                        float rotation,
                        ClassicLauncher::Color tint);

    void DrawTextEx(const ClassicLauncher::Font& font,
                    const char* text,
                    ClassicLauncher::Vector2f position,
                    float fontSize,
                    float spacing,
                    ClassicLauncher::Color tint);

    void DrawCircle(float x, float y, int radius, ClassicLauncher::Color color);

    // Inputs
    // --- Keyboard ---
    bool IsKeyPressed(int key);
    bool IsKeyDown(int key);
    bool IsKeyReleased(int key);
    bool IsKeyUp(int key);

    // --- Gamepads ---
    bool IsGamepadButtonPressed(int gamepad, int button);
    bool IsGamepadButtonDown(int gamepad, int button);
    bool IsGamepadButtonReleased(int gamepad, int button);
    bool IsGamepadButtonUp(int gamepad, int button);
    bool IsGamepadAvailable(int gamepad);

    // --- Mouse ---
    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonDown(int button);
    bool IsMouseButtonReleased(int button);
    bool IsMouseButtonUp(int button);

} // namespace rlw

#endif // RL_WRAP_H