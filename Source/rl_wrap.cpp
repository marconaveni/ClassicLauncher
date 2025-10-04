// rl_wrap.cpp: implementação do wrapper chamando raylib nativa.
// Este arquivo é o ÚNICO que inclui <raylib.h>.
#include "rl_wrap.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "raylib.h"

namespace rlw
{

    // --- Converters entre tipos rlw e raylib ---

    static ::Vector2 to_native_vec(ClassicLauncher::Vector2f v)
    {
        return ::Vector2{v.x, v.y};
    }
    static ClassicLauncher::Vector2f to_wrap_vec(::Vector2 v)
    {
        return ClassicLauncher::Vector2f{v.x, v.y};
    }

    static ::Color to_native_color(ClassicLauncher::Color c)
    {
        return ::Color{c.r, c.g, c.b, c.a};
    }
    static ClassicLauncher::Color to_wrap_color(::Color c)
    {
        return ClassicLauncher::Color{c.r, c.g, c.b, c.a};
    }

    static ::Rectangle to_native_rec(ClassicLauncher::RectFloat r)
    {
        return ::Rectangle{r.x, r.y, r.width, r.height};
    }
    static ClassicLauncher::RectFloat to_wrap_rec(::Rectangle r)
    {
        return ClassicLauncher::RectFloat{r.x, r.y, r.width, r.height};
    }


    // --- Logging / Config / Janela ---
    void SetTraceLogCallback(void (*callback)(int, const char*, va_list))
    {
        ::SetTraceLogCallback(callback);
    }
    void SetTraceLogLevel(int level)
    {
        ::SetTraceLogLevel(level);
    }


    // --- Desenho 2D ---
    void BeginDrawing()
    {
        ::BeginDrawing();
    }
    void EndDrawing()
    {
        ::EndDrawing();
    }
    void ClearBackground(ClassicLauncher::Color color)
    {
        ::ClearBackground(to_native_color(color));
    }

    // --- Entrada ---
    bool IsKeyPressed(int key)
    {
        return ::IsKeyPressed(key);
    }
    bool IsKeyDown(int key)
    {
        return ::IsKeyDown(key);
    }
    bool IsKeyReleased(int key)
    {
        return ::IsKeyReleased(key);
    }
    bool IsKeyUp(int key)
    {
        return ::IsKeyUp(key);
    }

    bool IsGamepadButtonPressed(int gamepad, int button)
    {
        return ::IsGamepadButtonPressed(gamepad, button);
    }
    bool IsGamepadButtonDown(int gamepad, int button)
    {
        return ::IsGamepadButtonDown(gamepad, button);
    }
    bool IsGamepadButtonReleased(int gamepad, int button)
    {
        return ::IsGamepadButtonReleased(gamepad, button);
    }
    bool IsGamepadButtonUp(int gamepad, int button)
    {
        return ::IsGamepadButtonUp(gamepad, button);
    }

    ClassicLauncher::Vector2f GetMousePosition()
    {
        return to_wrap_vec(::GetMousePosition());
    }

    // --- FS Utils ---
    // const char* GetApplicationDirectory()
    // {
    //     return ::GetApplicationDirectory();
    // }
    // const char* GetWorkingDirectory()
    // {
    //     return ::GetWorkingDirectory();
    // }
    // bool ChangeDirectory(const char* path)
    // {
    //     return ::ChangeDirectory(path);
    // }
    // const char* GetFileNameWithoutExt(const char* filePath)
    // {
    //     return ::GetFileNameWithoutExt(filePath);
    // }


    void BeginScissorMode(int x, int y, int width, int height)
    {
        ::BeginScissorMode(x, y, width, height);
    }
    void EndScissorMode()
    {
        ::EndScissorMode();
    }

    void DrawRectangle(int x, int y, int width, int height, ClassicLauncher::Color color)
    {
        ::DrawRectangle(x, y, width, height, to_native_color(color));
    }
    void DrawRectangleLinesEx(ClassicLauncher::RectFloat rec, float lineThick, ClassicLauncher::Color color)
    {
        ::DrawRectangleLinesEx(to_native_rec(rec), lineThick, to_native_color(color));
    }


    // const char* GetDirectoryPath(const char* filePath)
    // {
    //     return ::GetDirectoryPath(filePath);
    // }
    // bool FileExists(const char* fileName)
    // {
    //     return ::FileExists(fileName);
    // }
    // bool DirectoryExists(const char* dirPath)
    // {
    //     return ::DirectoryExists(dirPath);
    // }
    // bool MakeDirectory(const char* dirPath)
    // {
    //     return ::MakeDirectory(dirPath);
    // }

    // void WaitTime(double seconds)
    // {
    //     ::WaitTime(seconds);
    // }


    // --- Mouse ---
    bool IsMouseButtonPressed(int button)
    {
        return ::IsMouseButtonPressed(button);
    }


    //////////////////////////////////////////////

    void SetWindowIcons(ClassicLauncher::Image* images, int count)
    {
        // raylib espera ponteiro para Image nativo
        ::Image* rayImages = new ::Image[count];
        for (int i = 0; i < count; ++i)
        {
            ::Image rayImage{};
            rayImage.data = images->data;
            rayImage.width = images->width;
            rayImage.height = images->height;
            rayImage.mipmaps = images->mipmaps;
            rayImage.format = images->format;
            rayImages[i] = rayImage;
        }

        ::SetWindowIcons(rayImages, count);
        delete[] rayImages;
    }

    void BeginTextureMode(const ClassicLauncher::RenderTexture& target)
    {
        ::RenderTexture2D rayTarget;
        rayTarget.id = target.GetId();
        rayTarget.texture.id = target.GetTextureId();
        rayTarget.texture.width = target.GetSize().width;
        rayTarget.texture.height = target.GetSize().height;

        ::BeginTextureMode(rayTarget);
    }

    void EndTextureMode()
    {
        ::EndTextureMode();
    }

    void DrawTexture(const ClassicLauncher::Texture& texture, int posX, int posY, ClassicLauncher::Color tint)
    {
        ::Texture2D rayTex{};
        rayTex.id = texture.GetId();
        rayTex.width = texture.GetSize().width;
        rayTex.height = texture.GetSize().height;
        rayTex.mipmaps = texture.GetMipmaps();
        rayTex.format = texture.GetFormat();

        ::DrawTexture(rayTex, posX, posY, to_native_color(tint));
    }

    void DrawTexturePro(const ClassicLauncher::Texture& texture,
                        ClassicLauncher::RectFloat src,
                        ClassicLauncher::RectFloat dst,
                        ClassicLauncher::Vector2f origin,
                        float rotation,
                        ClassicLauncher::Color tint)
    {
        ::Texture2D rayTex{};
        rayTex.id = texture.GetId();
        rayTex.width = texture.GetSize().width;
        rayTex.height = texture.GetSize().height;
        rayTex.mipmaps = texture.GetMipmaps();
        rayTex.format = texture.GetFormat();
        ::DrawTexturePro(rayTex, to_native_rec(src), to_native_rec(dst), to_native_vec(origin), rotation,
                         to_native_color(tint));
    }

    void DrawTextEx(ClassicLauncher::Font font,
                    const char* text,
                    ClassicLauncher::Vector2f position,
                    float fontSize,
                    float spacing,
                    ClassicLauncher::Color tint)
    {
        if (!font._native || !text)
        {
            return;
        }
        ::DrawTextEx(*static_cast<::Font*>(font._native), text, to_native_vec(position), fontSize, spacing,
                     to_native_color(tint));
    }


} // namespace rlw