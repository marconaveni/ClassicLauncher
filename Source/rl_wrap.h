#ifndef RL_WRAP_H
#define RL_WRAP_H

// Wrapper minimalista para isolar a raylib.
// Incluir este header no projeto no lugar de <raylib.h>.
// A implementação (rl_wrap.cpp) inclui <raylib.h> e chama as funções nativas.
//
// Escopo: apenas funções observadas em uso no projeto.
// Namespace: rlw

#include <cstdarg>
#include "Data/Color.h"
#include "Data/Rectangle.h"
#include "Data/Vector2.h"
#include "Graphics/Image.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/Texture.h"
#include "Graphics/Font.h"

namespace rlw
{


    // --- Logging / Config / Janela ---
    void SetTraceLogCallback(void (*callback)(int logLevel, const char* text, va_list args));
    void SetTraceLogLevel(int logLevel);


    // --- Desenho 2D ---
    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(ClassicLauncher::Color color);



    // Scissor
    void BeginScissorMode(int x, int y, int width, int height);
    void EndScissorMode();

    // Retângulos
    void DrawRectangle(int x, int y, int width, int height, ClassicLauncher::Color color);
    void DrawRectangleLinesEx(ClassicLauncher::RectFloat rec, float lineThick, ClassicLauncher::Color color);
 

    // --- Entrada ---

    ClassicLauncher::Vector2f GetMousePosition();




    // --- FS Utils ---
    const char* GetApplicationDirectory();
    const char* GetWorkingDirectory();
    bool ChangeDirectory(const char* path);
    const char* GetFileNameWithoutExt(const char* filePath);



    


    const char* GetDirectoryPath(const char* filePath);
    // bool FileExists(const char* fileName);
    //bool DirectoryExists(const char* dirPath);
    //bool MakeDirectory(const char* dirPath);

    void WaitTime(double seconds);


    //////////////////ainda em uso 

    void BeginTextureMode(const ClassicLauncher::RenderTexture& target);
    void EndTextureMode();


    // Texturas simples e "pro"
    void DrawTexture(const ClassicLauncher::Texture& texture, int posX, int posY, ClassicLauncher::Color tint);
    void DrawTexturePro(const ClassicLauncher::Texture& texture,
                        ClassicLauncher::RectFloat src,
                        ClassicLauncher::RectFloat dst,
                        ClassicLauncher::Vector2f origin,
                        float rotation,
                        ClassicLauncher::Color tint);

    
    void DrawTextEx(ClassicLauncher::Font font,
                    const char* text,
                    ClassicLauncher::Vector2f position,
                    float fontSize,
                    float spacing,
                    ClassicLauncher::Color tint);
    


}  // namespace rlw

#endif  // RL_WRAP_H