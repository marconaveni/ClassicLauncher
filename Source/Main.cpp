#include "Application.h"

int main()
{
   ClassicLauncher::Application application;
   application.Init();
}

// #include "Graphics/RenderTexture.h"
// #include "raylib.h"

// int main(void)
// {
//     // Inicializa janela
//     const int screenWidth = 800;
//     const int screenHeight = 450;
//     InitWindow(screenWidth, screenHeight, "Exemplo RenderTexture - Raylib");

//     // Cria um RenderTexture

//     ClassicLauncher::RenderTexture myrender(screenWidth - 30,screenHeight - 30);
//     RenderTexture2D target;// = LoadRenderTexture(screenWidth, screenHeight);
//     target.id = myrender.GetId();
//     target.texture.id = myrender.GetTextureId();
//     target.texture.width = myrender.GetSize().x;
//     target.texture.height = myrender.GetSize().y;

//     // Define FPS
//     SetTargetFPS(60);

//     while (!WindowShouldClose())
//     {
//         // --- Desenhando no RenderTexture ---

//         BeginTextureMode(target);         // Tudo aqui vai pro render target
//             ClearBackground(RAYWHITE);
//             DrawCircle(screenWidth/2, screenHeight/2, 100, RED);
//             DrawText("Desenhado no RenderTexture!", 190, 200, 20, DARKBLUE);
//         EndTextureMode();

//         // --- Desenhando na tela ---
//         BeginDrawing();
//             ClearBackground(BLACK);

//             // Importante: a textura vem invertida no eixo Y, então flip vertical
//             DrawTextureRec(
//                 target.texture,
//                 (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
//                 (Vector2){ 0, 0 },
//                 WHITE
//             );

//             DrawText("Renderizado na tela", 10, 10, 20, GREEN);
//         EndDrawing();
//     }

//     // Limpeza
//     UnloadRenderTexture(target);
//     CloseWindow();

//     return 0;
// }

// #include "raylib.h"

// #include "Graphics/Texture.h"
// #include "Utils/Log.h"

// int main()
// {
//     InitWindow(800, 450, "raylib [core] example - basic window");

//     ClassicLauncher::Texture texture("c:/Projetos/EngineOpengl Raylib Comp/assets/1test.png");
//     //Image im = LoadImage("c:/Projetos/EngineOpengl Raylib Comp/assets/1test.png");
//     //texture.LoadFromData(im.data,im.width, im.height, im.mipmaps,im.format);
//     //Texture2D tex = Texture2D{ texture.GetId(), texture.GetSize().x, texture.GetSize().y, texture.GetMipmaps(), texture.GetFormat() };

//     //Texture2D texture =  LoadTexture("c:/Projetos/EngineOpengl Raylib Comp/assets/1test.png");

//     while (!WindowShouldClose())
//     {
//         BeginDrawing();
//         ClearBackground(RAYWHITE);
//         DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

//         DrawTexture({ texture.GetId(), texture.GetSize().x, texture.GetSize().y, texture.GetMipmaps(), texture.GetFormat() }, 20, 20,
//         WHITE);
//         //DrawTexture(texture, 20, 20, WHITE);

//         EndDrawing();
//     }

//     CloseWindow();
// }
