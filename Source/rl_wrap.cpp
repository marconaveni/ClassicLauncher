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

    static ::Image to_native_img(Image im)
    {
        ::Image n{};
        n.data = im.data;
        n.width = im.width;
        n.height = im.height;
        n.mipmaps = im.mipmaps;
        n.format = im.format;
        return n;
    }
    static Image to_wrap_img(::Image im)
    {
        Image w{};
        w.data = im.data;
        w.width = im.width;
        w.height = im.height;
        w.mipmaps = im.mipmaps;
        w.format = im.format;
        return w;
    }

    static ::Texture2D to_native_texture(Texture2D t)
    {
        ::Texture2D n{};
        n.id = t.id;
        n.width = t.width;
        n.height = t.height;
        n.mipmaps = t.mipmaps;
        n.format = t.format;
        return n;
    }
    static Texture2D to_wrap_texture(::Texture2D t)
    {
        Texture2D w{};
        w.id = t.id;
        w.width = t.width;
        w.height = t.height;
        w.mipmaps = t.mipmaps;
        w.format = t.format;
        return w;
    }

    static ::RenderTexture2D to_native_render_texture(RenderTexture2D r)
    {
        ::RenderTexture2D n{};
        n.id = r.id;
        n.texture = to_native_texture(r.texture);
        n.depth = to_native_texture(r.depth);
        return n;
    }
    static RenderTexture2D to_wrap_render_texture(::RenderTexture2D r)
    {
        RenderTexture2D w{};
        w.id = r.id;
        w.texture = to_wrap_texture(r.texture);
        w.depth = to_wrap_texture(r.depth);
        return w;
    }

    static ::AudioStream to_native_audio_stream(AudioStream s)
    {
        ::AudioStream n{};
        n.buffer = reinterpret_cast<::rAudioBuffer*>(s.buffer);
        n.sampleRate = s.sampleRate;
        n.sampleSize = s.sampleSize;
        n.channels = s.channels;
        return n;
    }
    static AudioStream to_wrap_audio_stream(::AudioStream s)
    {
        AudioStream w{};
        w.buffer = reinterpret_cast<void*>(s.buffer);
        w.sampleRate = s.sampleRate;
        w.sampleSize = s.sampleSize;
        w.channels = s.channels;
        return w;
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
    void SetWindowIcons(Image* images, int count)
    {
        // raylib espera ponteiro para Image nativo
        ::Image* native = new ::Image[count];
        for (int i = 0; i < count; ++i) native[i] = to_native_img(images[i]);
        ::SetWindowIcons(native, count);
        delete[] native;
    }
    void SetExitKey(int key)
    {
        ::SetExitKey(key);
    }
    void SetTargetFPS(int fps)
    {
        ::SetTargetFPS(fps);
    }
    int GetFPS()
    {
        return ::GetFPS();
    }
    float GetFrameTime()
    {
        return ::GetFrameTime();
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

    // --- Imagem / Textura ---
    Image LoadImage(const char* fileName)
    {
        return to_wrap_img(::LoadImage(fileName));
    }
    void UnloadImage(Image image)
    {
        ::UnloadImage(to_native_img(image));
    }
    void ImageResize(Image* image, int newWidth, int newHeight)
    {
        ::Image tmp = to_native_img(*image);
        ::ImageResize(&tmp, newWidth, newHeight);
        *image = to_wrap_img(tmp);
    }
    void ImageResizeNN(Image* image, int newWidth, int newHeight)
    {
        ::Image tmp = to_native_img(*image);
        ::ImageResizeNN(&tmp, newWidth, newHeight);
        *image = to_wrap_img(tmp);
    }

    Texture2D LoadTextureFromImage(Image image)
    {
        return to_wrap_texture(::LoadTextureFromImage(to_native_img(image)));
    }
    void UnloadTexture(Texture2D texture)
    {
        ::UnloadTexture(to_native_texture(texture));
    }
    void SetTextureFilter(Texture2D texture, int filter)
    {
        ::SetTextureFilter(to_native_texture(texture), filter);
    }

    // --- Render alvo ---
    RenderTexture2D LoadRenderTexture(int width, int height)
    {
        return to_wrap_render_texture(::LoadRenderTexture(width, height));
    }
    void UnloadRenderTexture(RenderTexture2D target)
    {
        ::UnloadRenderTexture(to_native_render_texture(target));
    }
    bool IsRenderTextureValid(RenderTexture2D target)
    {
        return ::IsRenderTextureValid(to_native_render_texture(target));
    }

    void BeginTextureMode(RenderTexture2D target)
    {
        ::BeginTextureMode(to_native_render_texture(target));
    }
    void EndTextureMode()
    {
        ::EndTextureMode();
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

    void DrawTexturePro(Texture2D texture,
                        float srcX,
                        float srcY,
                        float srcW,
                        float srcH,
                        float dstX,
                        float dstY,
                        float dstW,
                        float dstH,
                        float originX,
                        float originY,
                        float rotation)
    {
        ::Rectangle src{ srcX, srcY, srcW, srcH };
        ::Rectangle dst{ dstX, dstY, dstW, dstH };
        ::Vector2 origin{ originX, originY };
        ::DrawTexturePro(to_native_texture(texture), src, dst, origin, rotation, ::WHITE);
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

    // --- Áudio ---
    void InitAudioDevice()
    {
        ::InitAudioDevice();
    }
    void CloseAudioDevice()
    {
        ::CloseAudioDevice();
    }

    Music LoadMusicStream(const char* fileName)
    {
        ::Music* nm = new ::Music(::LoadMusicStream(fileName));  // copia o retornado
        rlw::Music w{};
        w.frameCount = nm->frameCount;
        w.looping = nm->looping;
        w.ctxType = nm->ctxType;
        w.ctxData = nm->ctxData;
        w._native = nm;
        return w;
    }
    bool IsMusicValid(Music music)
    {
        if (!music._native) return false;
        return ::IsMusicValid(*reinterpret_cast<::Music*>(music._native));
    }
    void UnloadMusicStream(Music music)
    {
        if (!music._native) return;
        ::UnloadMusicStream(*reinterpret_cast<::Music*>(music._native));
        delete reinterpret_cast<::Music*>(music._native);
    }

    void PlayMusicStream(Music music)
    {
        if (music._native) ::PlayMusicStream(*reinterpret_cast<::Music*>(music._native));
    }
    void PauseMusicStream(Music music)
    {
        if (music._native) ::PauseMusicStream(*reinterpret_cast<::Music*>(music._native));
    }
    void StopMusicStream(Music music)
    {
        if (music._native) ::StopMusicStream(*reinterpret_cast<::Music*>(music._native));
    }
    void SeekMusicStream(Music music, float position)
    {
        if (music._native) ::SeekMusicStream(*reinterpret_cast<::Music*>(music._native), position);
    }
    void UpdateMusicStream(Music music)
    {
        if (music._native) ::UpdateMusicStream(*reinterpret_cast<::Music*>(music._native));
    }

    float GetMusicTimeLength(Music music)
    {
        if (!music._native) return 0.0f;
        return ::GetMusicTimeLength(*reinterpret_cast<::Music*>(music._native));
    }
    float GetMusicTimePlayed(Music music)
    {
        if (!music._native) return 0.0f;
        return ::GetMusicTimePlayed(*reinterpret_cast<::Music*>(music._native));
    }

    Sound LoadSound(const char* fileName)
    {
        ::Sound* ns = new ::Sound(::LoadSound(fileName));
        Sound w{};
        w.frameCount = ns->frameCount;
        w._native = ns;
        return w;
    }
    bool IsSoundValid(Sound sound)
    {
        if (!sound._native) return false;
        return ::IsSoundValid(*reinterpret_cast<::Sound*>(sound._native));
    }
    void PlaySound(Sound sound)
    {
        if (sound._native) ::PlaySound(*reinterpret_cast<::Sound*>(sound._native));
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

        if ((rec1.x < (rec2.x + rec2.width) && (rec1.x + rec1.width) > rec2.x) && (rec1.y < (rec2.y + rec2.height) && (rec1.y + rec1.height) > rec2.y))
        {
            collision = true;
        }

        return collision;
    }

    void DrawTexture(Texture2D texture, int posX, int posY, ClassicLauncher::Color tint)
    {
        ::DrawTexture(to_native_texture(texture), posX, posY, to_native_color(tint));
    }

    void DrawTexturePro(Texture2D texture, ClassicLauncher::RectFloat src, ClassicLauncher::RectFloat dst, ClassicLauncher::Vector2f origin, float rotation, ClassicLauncher::Color tint)
    {
        ::DrawTexturePro(to_native_texture(texture), to_native_rec(src), to_native_rec(dst), to_native_vec(origin), rotation, to_native_color(tint));
    }

    Font GetFontDefault()
    {
        static ::Font s_default_native = ::GetFontDefault();

        rlw::Font w{};
        w.baseSize = s_default_native.baseSize;
        w.glyphCount = s_default_native.glyphCount;
        w.glyphPadding = s_default_native.glyphPadding;
        w._native = &s_default_native;  // ponteiro estável para a estática
        w._owned = false;               // NÃO descarregar: é o default
        return w;
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

    void* MemAlloc(unsigned int size)
    {
        return ::MemAlloc(size);
    }
    void MemFree(void* ptr)
    {
        ::MemFree(ptr);
    }

    Image MakeImage(void* data, int width, int height, int mipmaps, int format)
    {
        ::Image n{};
        n.data = data;
        n.width = width;
        n.height = height;
        n.mipmaps = mipmaps;
        n.format = format;
        return to_wrap_img(n);
    }

    // --- Imagem / Textura utilitários extras ---
    bool IsImageValid(Image image)
    {
        return ::IsImageValid(to_native_img(image));
    }
    bool IsTextureValid(Texture2D tex)
    {
        return ::IsTextureValid(to_native_texture(tex));
    }

    Image GenImageColor(int width, int height, ClassicLauncher::Color color)
    {
        return to_wrap_img(::GenImageColor(width, height, to_native_color(color)));
    }

    Image ImageCopy(Image src)
    {
        return to_wrap_img(::ImageCopy(to_native_img(src)));
    }

    void UpdateTexture(Texture2D texture, const void* pixels)
    {
        ::UpdateTexture(to_native_texture(texture), pixels);
    }

    // --- Fonte / Texto ---
    bool IsFontValid(Font font)
    {
        if (!font._native) return false;
        return ::IsFontValid(*static_cast<::Font*>(font._native));
    }
    void UnloadFont(Font font)
    {
        if (!font._native) return;
        if (font._owned)
        {
            ::UnloadFont(*static_cast<::Font*>(font._native));
            delete static_cast<::Font*>(font._native);
        }
        // se não é dono (default), não faz nada
    }

    Font LoadFontEx(const char* fileName, int fontSize, int* codepoints, int codepointCount)
    {
        ::Font* nf = new ::Font(::LoadFontEx(fileName, fontSize, codepoints, codepointCount));
        rlw::Font w{};
        w.baseSize = nf->baseSize;
        w.glyphCount = nf->glyphCount;
        w.glyphPadding = nf->glyphPadding;
        w._native = nf;
        w._owned = true;  // nós alocamos, então descarregamos depois
        return w;
    }

    void DrawTextEx(Font font, const char* text, ClassicLauncher::Vector2f position, float fontSize, float spacing, ClassicLauncher::Color tint)
    {
        if (!font._native || !text) return;
        ::DrawTextEx(*static_cast<::Font*>(font._native), text, to_native_vec(position), fontSize, spacing, to_native_color(tint));
    }

    ClassicLauncher::Vector2f MeasureTextEx(Font font, const char* text, float fontSize, float spacing)
    {
        if (!font._native || !text) return ClassicLauncher::Vector2f{ 0, 0 };
        return to_wrap_vec(::MeasureTextEx(*static_cast<::Font*>(font._native), text, fontSize, spacing));
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

    bool IsAudioDeviceReady()
    {
        return ::IsAudioDeviceReady();
    }

    void LogMusicState(const rlw::Music& m)
    {
        if (!m._native)
        {
            std::printf("[Music] null\n");
            return;
        }
        auto& nm = *reinterpret_cast<::Music*>(m._native);
        std::printf("[Music] frameCount=%u looping=%d ctxType=%d ctxData=%p | stream=%p\n", nm.frameCount, nm.looping ? 1 : 0, nm.ctxType, nm.ctxData, nm.stream.buffer);
        std::printf("        time len=%.3f played=%.3f\n", ::GetMusicTimeLength(nm), ::GetMusicTimePlayed(nm));
    }

}  // namespace rlw