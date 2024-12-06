cmake_minimum_required(VERSION 3.16)
project(ClassicLauncher LANGUAGES CXX)

# Configurações de compilação
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona diretórios de cabeçalhos
include_directories(
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/Source/Utils
    ${CMAKE_SOURCE_DIR}/Source/Graphics
    ${CMAKE_SOURCE_DIR}/Source/Audio
)

# Lista de arquivos fonte
set(SOURCES
    Source/Main.cpp
    Source/Application.cpp
    Source/Audio/AudioManager.cpp
    Source/Graphics/Render.cpp
    Source/Graphics/Sprite.cpp
    Source/Utils/Print.cpp
    Source/Utils/UtilsFunctionLibrary.cpp
    Source/Utils/StringFunctionLibrary.cpp
)

# Adiciona o executável
add_executable(ClassicLauncher ${SOURCES})

# Configura bibliotecas e opções específicas do sistema operacional
if(WIN32)
    # Diretório da Raylib no Windows
    set(RAYLIB_DIR "C:/libs/raylib")
    include_directories(${RAYLIB_DIR}/include)
    target_link_directories(ClassicLauncher PRIVATE ${RAYLIB_DIR}/lib)

    # Bibliotecas para Windows
    target_link_libraries(ClassicLauncher raylib opengl32 gdi32 winmm)
else()
    # Bibliotecas para Linux
    target_link_libraries(ClassicLauncher raylib GL m pthread dl rt vlc)
endif()
