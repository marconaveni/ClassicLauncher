#include "Application.h"

int main()
{
    ClassicLauncher::Application application;
    application.Init();
}

// #include <string>
// #include "raylib.h"

// int main()
// {
//     // Inicializa a janela
//     InitWindow(800, 600, "Carregar Fonte na Raylib");

//     // Carrega uma fonte personalizada
//     Font font = LoadFontEx("/home/marco/Projetos/ClassicLauncher/Resources/fonts/roboto.ttf", 48, NULL, 0);  // Substitua pelo caminho para a sua fonte
//     // Font font = LoadFont("/home/marco/Projetos/ClassicLauncher/Resources/fonts/roboto.ttf"); // Substitua pelo caminho para a sua fonte
//     //  Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
//     // Font font = LoadFontFromMemory(".ttf", fontData_Roboto, COMPRESSED_DATA_SIZE_FONT_ROBOTO, 48, NULL, 0);
//     // Font font = LoadFont_Roboto();

//     // Configuração da mensagem a ser exibida
//     std::string message = "Ola, Raylib com fontes personalizadas!";

//     // Cor do texto
//     Color textColor = DARKBLUE;

//     // Loop principal
//     while (!WindowShouldClose())
//     {
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         // Usa a fonte carregada para desenhar o texto
//         DrawTextEx(font, message.c_str(), (Vector2){ 100, 200 }, 48, 2, textColor);

//         EndDrawing();
//     }

//     // Limpa os recursos usados
//     // UnloadFont(font);
//     CloseWindow();

//     return 0;
// }
