// rl_wrap.cpp: implementação do wrapper chamando raylib nativa.
// Este arquivo é o ÚNICO que inclui <raylib.h>.
#include "rl_wrap.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include "raylib.h"
#include "raymath.h"

namespace rlw
{

    // --- Converters entre tipos rlw e raylib ---

    static ::Vector2 to_native_vec(ClassicLauncher::Vector2f v)
    {
        return { v.x, v.y };
    }
    static ClassicLauncher::Vector2f to_wrap_vec(::Vector2 v)
    {
        return { v.x, v.y };
    }

    static ::Color to_native_color(ClassicLauncher::Color c)
    {
        return { c.r, c.g, c.b, c.a };
    }
    static ClassicLauncher::Color to_wrap_color(::Color c)
    {
        return { c.r, c.g, c.b, c.a };
    }

    static ::Rectangle to_native_rec(ClassicLauncher::RectFloat r)
    {
        return { r.x, r.y, r.width, r.height };
    }
    static ClassicLauncher::RectFloat to_wrap_rec(::Rectangle r)
    {
        return { r.x, r.y, r.width, r.height };
    }

    static ::FilePathList to_native_path(FilePathList f)
    {
        ::FilePathList n{};
        n.count = f.count;
        n.paths = f.paths;
        return n;
    }
    static FilePathList to_wrap_path(::FilePathList f)
    {
        FilePathList w{};
        w.count = f.count;
        w.paths = f.paths;
        return w;
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







    void SetConfigFlags(unsigned int flags)
    {
        ::SetConfigFlags(flags);
    }

    void SetWindowState(unsigned int flags)
    {
        ::SetWindowState(flags);
    }
    void ClearWindowState(unsigned int flags)
    {
        ::ClearWindowState(flags);
    }
    bool IsWindowState(unsigned int flag)
    {
        return ::IsWindowState(flag);
    }
    void SetWindowSize(int w, int h)
    {
        ::SetWindowSize(w, h);
    }
    void SetWindowPosition(int x, int y)
    {
        ::SetWindowPosition(x, y);
    }

    void SetExitKey(int key)
    {
        ::SetExitKey(key);
    }
    void SetTargetFPS(int fps)
    {
        ::SetTargetFPS(fps);
    }


    int GetScreenWidth()
    {
        return ::GetScreenWidth();
    }
    int GetScreenHeight()
    {
        return ::GetScreenHeight();
    }

    int GetCurrentMonitor()
    {
        return ::GetCurrentMonitor();
    }
    int GetMonitorWidth(int m)
    {
        return ::GetMonitorWidth(m);
    }
    int GetMonitorHeight(int m)
    {
        return ::GetMonitorHeight(m);
    }
    ClassicLauncher::Vector2f GetMonitorPosition(int m)
    {
        return to_wrap_vec(::GetMonitorPosition(m));
    }

    ClassicLauncher::Vector2f GetWindowPosition()
    {
        return to_wrap_vec(::GetWindowPosition());
    }
    bool IsWindowFullscreen()
    {
        return ::IsWindowFullscreen();
    }
    void ToggleFullscreen()
    {
        ::ToggleFullscreen();
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

    // --- Math (raymath) ---
    ClassicLauncher::Vector2f Vector2Clamp(ClassicLauncher::Vector2f value, ClassicLauncher::Vector2f min, ClassicLauncher::Vector2f max)
    {
        return to_wrap_vec(::Vector2Clamp(to_native_vec(value), to_native_vec(min), to_native_vec(max)));
    }

    // --- FS Utils ---
    const char* GetApplicationDirectory()
    {
        return ::GetApplicationDirectory();
    }
    const char* GetWorkingDirectory()
    {
        return ::GetWorkingDirectory();
    }
    bool ChangeDirectory(const char* path)
    {
        return ::ChangeDirectory(path);
    }
    const char* GetFileNameWithoutExt(const char* filePath)
    {
        return ::GetFileNameWithoutExt(filePath);
    }

    // --- Directory listing ---
    FilePathList LoadDirectoryFiles(const char* dirPath)
    {
        return to_wrap_path(::LoadDirectoryFiles(dirPath));
    }
    void UnloadDirectoryFiles(FilePathList files)
    {
        ::UnloadDirectoryFiles(to_native_path(files));
    }

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

    bool CheckCollisionPointRec(ClassicLauncher::Vector2f point, ClassicLauncher::RectFloat rec)
    {
        bool collision = false;

        if ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height)))
        {
            collision = true;
        }

        return collision;
    }
    bool CheckCollisionRecs(ClassicLauncher::RectFloat rec1, ClassicLauncher::RectFloat rec2)
    {
        bool collision = false;

        if ((rec1.x < (rec2.x + rec2.width) && (rec1.x + rec1.width) > rec2.x) &&
            (rec1.y < (rec2.y + rec2.height) && (rec1.y + rec1.height) > rec2.y))
        {
            collision = true;
        }

        return collision;
    }

    const char* GetDirectoryPath(const char* filePath)
    {
        return ::GetDirectoryPath(filePath);
    }
    bool FileExists(const char* fileName)
    {
        return ::FileExists(fileName);
    }
    bool DirectoryExists(const char* dirPath)
    {
        return ::DirectoryExists(dirPath);
    }
    bool MakeDirectory(const char* dirPath)
    {
        return ::MakeDirectory(dirPath);
    }

    void WaitTime(double seconds)
    {
        ::WaitTime(seconds);
    }


    // --- Mouse ---
    bool IsMouseButtonPressed(int button)
    {
        return ::IsMouseButtonPressed(button);
    }

    const char* TextFormat(const char* text, ...)
    {
        const int maxTextFormatBuffers = 4;  // Maximum number of static buffers for text formatting

        const int maxTextBufferLen = 1024;

        static char buffers[maxTextFormatBuffers][maxTextBufferLen] = { 0 };
        static int index = 0;

        char* currentBuffer = buffers[index];
        memset(currentBuffer, 0, maxTextBufferLen);

        std::va_list args;
        va_start(args, text);
        int requiredByteCount = std::vsnprintf(currentBuffer, maxTextBufferLen, text, args);
        va_end(args);

        if (requiredByteCount >= maxTextBufferLen)
        {
            char* truncBuffer = buffers[index] + maxTextBufferLen - 4;  // Adding 4 bytes = "...\0"
            std::sprintf(truncBuffer, "...");
        }

        index += 1;  // Move to next buffer for next function call
        if (index >= maxTextFormatBuffers) index = 0;

        return currentBuffer;
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
        rayTarget.texture.width = target.GetSize().x;
        rayTarget.texture.height = target.GetSize().y;

        ::BeginTextureMode(rayTarget);
    }

    void EndTextureMode()
    {
        ::EndTextureMode();
    }

    ///////////////////////////////////////////////
    //                                           //
    //                                           //
    //                                           //
    //                                           //
    //                                           //
    ///////////////////////////////////////////////

    void DrawTexture(const ClassicLauncher::Texture& texture, int posX, int posY, ClassicLauncher::Color tint)
    {
        ::Texture2D rayTex{};
        rayTex.id = texture.GetId();
        rayTex.width = texture.GetSize().x;
        rayTex.height = texture.GetSize().y;
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
        rayTex.width = texture.GetSize().x;
        rayTex.height = texture.GetSize().y;
        rayTex.mipmaps = texture.GetMipmaps();
        rayTex.format = texture.GetFormat();
        ::DrawTexturePro(rayTex, to_native_rec(src), to_native_rec(dst), to_native_vec(origin), rotation, to_native_color(tint));
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////

    // ClassicLauncher::Font GetFontDefault()
    // {
    //     static ::Font s_default_native = ::GetFontDefault();

    //     ClassicLauncher::Font w{};
    //     w.baseSize = s_default_native.baseSize;
    //     w.glyphCount = s_default_native.glyphCount;
    //     w.glyphPadding = s_default_native.glyphPadding;
    //     w._native = &s_default_native;  // ponteiro estável para a estática
    //     w._owned = false;               // NÃO descarregar: é o default
    //     return w;
    // }

    // --- Fonte / Texto ---
    // bool IsFontValid(ClassicLauncher::Font font)
    // {
    //     if (!font._native) return false;
    //     return ::IsFontValid(*static_cast<::Font*>(font._native));
    // }

    // void UnloadFont(ClassicLauncher::Font font)
    // {
    //     if (!font._native) return;
    //     if (font._owned)
    //     {
    //         ::UnloadFont(*static_cast<::Font*>(font._native));
    //         delete static_cast<::Font*>(font._native);
    //     }
    //     // se não é dono (default), não faz nada
    // }

    // ClassicLauncher::Font LoadFontEx(const char* fileName, int fontSize, int* codepoints, int codepointCount)
    // {
    //     ::Font* nf = new ::Font(::LoadFontEx(fileName, fontSize, codepoints, codepointCount));
    //     ClassicLauncher::Font w{};
    //     w.baseSize = nf->baseSize;
    //     w.glyphCount = nf->glyphCount;
    //     w.glyphPadding = nf->glyphPadding;
    //     w._native = nf;
    //     w._owned = true;  // nós alocamos, então descarregamos depois
    //     return w;
    // }

    void DrawTextEx(ClassicLauncher::Font font,
                    const char* text,
                    ClassicLauncher::Vector2f position,
                    float fontSize,
                    float spacing,
                    ClassicLauncher::Color tint)
    {
        if (!font._native || !text) return;
        ::DrawTextEx(*static_cast<::Font*>(font._native), text, to_native_vec(position), fontSize, spacing, to_native_color(tint));
    }

    // ClassicLauncher::Vector2f MeasureTextEx(ClassicLauncher::Font font, const char* text, float fontSize, float spacing)
    // {
    //     if (!font._native || !text) return ClassicLauncher::Vector2f{ 0, 0 };
    //     return to_wrap_vec(::MeasureTextEx(*static_cast<::Font*>(font._native), text, fontSize, spacing));
    // }

}  // namespace rlw