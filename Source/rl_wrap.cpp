// rl_wrap.cpp: implementação do wrapper chamando raylib nativa.
// Este arquivo é o ÚNICO que inclui <raylib.h>.
#include "rl_wrap.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace ray
{
#include "raylib.h"
} // namespace ray


namespace rlw
{

    // --- Converters between classiclauncher and raylib types ---
    static ray::Vector2 ToNativeVec(ClassicLauncher::Vector2f vector)
    {
        return ray::Vector2{vector.x, vector.y};
    }
    static ClassicLauncher::Vector2f ToWrapVec(ray::Vector2 vector)
    {
        return ClassicLauncher::Vector2f{vector.x, vector.y};
    }

    static ray::Color ToNativeColor(ClassicLauncher::Color color)
    {
        return ray::Color{color.r, color.g, color.b, color.a};
    }
    static ClassicLauncher::Color ToWrapColor(ray::Color color)
    {
        return ClassicLauncher::Color{color.r, color.g, color.b, color.a};
    }

    static ray::Rectangle ToNativeRec(ClassicLauncher::RectFloat rectangle)
    {
        return ray::Rectangle{rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    }
    // static ClassicLauncher::RectFloat ToWrapRec(ray::Rectangle rectangle)
    // {
    //     return ClassicLauncher::RectFloat{rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    // }


    // --- Logging / Configurations ---
    void SetTraceLogCallback(void (*callback)(int, const char*, va_list))
    {
        ray::SetTraceLogCallback(callback);
    }
    void SetTraceLogLevel(int level)
    {
        ray::SetTraceLogLevel(level);
    }

    // --- KeyBoard ---
    bool IsKeyPressed(int key)
    {
        return ray::IsKeyPressed(key);
    }
    bool IsKeyDown(int key)
    {
        return ray::IsKeyDown(key);
    }
    bool IsKeyReleased(int key)
    {
        return ray::IsKeyReleased(key);
    }
    bool IsKeyUp(int key)
    {
        return ray::IsKeyUp(key);
    }

    // --- GamePad ---
    bool IsGamepadButtonPressed(int gamepad, int button)
    {
        return ray::IsGamepadButtonPressed(gamepad, button);
    }

    bool IsGamepadButtonDown(int gamepad, int button)
    {
        return ray::IsGamepadButtonDown(gamepad, button);
    }

    bool IsGamepadButtonReleased(int gamepad, int button)
    {
        return ray::IsGamepadButtonReleased(gamepad, button);
    }

    bool IsGamepadButtonUp(int gamepad, int button)
    {
        return ray::IsGamepadButtonUp(gamepad, button);
    }

    bool IsGamepadAvailable(int gamepad)
    {
        return ray::IsGamepadAvailable(gamepad);
    }

    float GetGamepadAxisMovement(int gamepad, int axis)
    {
        return ray::GetGamepadAxisMovement(gamepad, axis);
    }

    // --- Mouse ---
    bool IsMouseButtonPressed(int button)
    {
        return ray::IsMouseButtonPressed(button);
    }

    bool IsMouseButtonDown(int button)
    {
        return ray::IsMouseButtonDown(button);
    }

    bool IsMouseButtonReleased(int button)
    {
        return ray::IsMouseButtonReleased(button);
    }

    bool IsMouseButtonUp(int button)
    {
        return ray::IsMouseButtonUp(button);
    }

    void BeginScissorMode(int x, int y, int width, int height)
    {
        ray::BeginScissorMode(x, y, width, height);
    }
    void EndScissorMode()
    {
        ray::EndScissorMode();
    }

    void DrawRectangle(int x, int y, int width, int height, ClassicLauncher::Color color)
    {
        ray::DrawRectangle(x, y, width, height, ToNativeColor(color));
    }

    void DrawRectangleLinesEx(ClassicLauncher::RectFloat rec, float lineThick, ClassicLauncher::Color color)
    {
        ray::DrawRectangleLinesEx(ToNativeRec(rec), lineThick, ToNativeColor(color));
    }

    // --- Drawing 2D ---
    void BeginDrawing()
    {
        ray::BeginDrawing();
    }
    void EndDrawing()
    {
        ray::EndDrawing();
    }

    void ClearBackground(ClassicLauncher::Color color)
    {
        ray::ClearBackground(ToNativeColor(color));
    }

    void BeginTextureMode(const ClassicLauncher::RenderTexture& target)
    {
        ray::RenderTexture2D rayTarget;
        rayTarget.id = target.GetId();
        rayTarget.texture.id = target.GetTextureId();
        rayTarget.texture.width = target.GetSize().width;
        rayTarget.texture.height = target.GetSize().height;

        ray::BeginTextureMode(rayTarget);
    }

    void EndTextureMode()
    {
        ray::EndTextureMode();
    }

    void DrawTexture(const ClassicLauncher::Texture& texture, int posX, int posY, ClassicLauncher::Color tint)
    {
        ray::Texture2D rayTex{};
        rayTex.id = texture.GetId();
        rayTex.width = texture.GetSize().width;
        rayTex.height = texture.GetSize().height;
        rayTex.mipmaps = texture.GetMipmaps();
        rayTex.format = texture.GetFormat();

        ray::DrawTexture(rayTex, posX, posY, ToNativeColor(tint));
    }

    void DrawTexturePro(const ClassicLauncher::Texture& texture,
                        ClassicLauncher::RectFloat src,
                        ClassicLauncher::RectFloat dst,
                        ClassicLauncher::Vector2f origin,
                        float rotation,
                        ClassicLauncher::Color tint)
    {
        ray::Texture2D rayTex{};
        rayTex.id = texture.GetId();
        rayTex.width = texture.GetSize().width;
        rayTex.height = texture.GetSize().height;
        rayTex.mipmaps = texture.GetMipmaps();
        rayTex.format = texture.GetFormat();
        ray::DrawTexturePro(rayTex, ToNativeRec(src), ToNativeRec(dst), ToNativeVec(origin), rotation, ToNativeColor(tint));
    }

    void DrawTextEx(const ClassicLauncher::Font& font,
                    const char* text,
                    ClassicLauncher::Vector2f position,
                    float fontSize,
                    float spacing,
                    ClassicLauncher::Color tint)
    {
        if (!font.GetNativeFont() || !text)
        {
            return;
        }
        ray::DrawTextEx(*static_cast<ray::Font*>(font.GetNativeFont()), text, ToNativeVec(position), fontSize, spacing, ToNativeColor(tint));
    }

    void DrawCircle(float x, float y, int radius, ClassicLauncher::Color color)
    {
        ray::DrawCircle(x, y, radius, ToNativeColor(color));
    }


} // namespace rlw