#include "Application.h"

int main()
{
   ClassicLauncher::Application application;
   application.Init();
}




// #include "raylib.h"
// #include <iostream>
// #include <format>
// #include <filesystem>
// #include <string>

// int main(){

//         //FilePathList files = LoadDirectoryFiles("C:/Users/Marco/.ClassicLauncher/musics");
//         std::filesystem::path directorypath = "C:/Users/Marco/.ClassicLauncher/musics";
//         for (const auto& entry : std::filesystem::directory_iterator(directorypath)) {
//             // Output the path of the file or subdirectory
//             std::cout << "File: " << entry.path() << "\n";
//         }
//         //ChangeMusic(bAutoPlay);


// }
