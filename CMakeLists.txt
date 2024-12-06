cmake_minimum_required(VERSION 3.16)
project(ClassicLauncher LANGUAGES CXX)

# Configurações de compilação
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona diretórios de cabeçalhos
include_directories(
    ${CMAKE_SOURCE_DIR}/Source
)

# Lista de arquivos fonte
set(SOURCES
    Source/Main.cpp
    Source/Application.cpp
    Source/Application.h
    Source/Audio/AudioManager.cpp
    Source/Audio/AudioManager.h
    Source/Graphics/Render.cpp
    Source/Graphics/Render.h
    Source/Graphics/Sprite.cpp
    Source/Graphics/Sprite.h
    Source/Utils/Core.h
    Source/Utils/Math.h
    Source/Utils/Print.cpp
    Source/Utils/Print.h
    Source/Utils/Resouces.h
    Source/Utils/StringFunctionLibrary.cpp
    Source/Utils/StringFunctionLibrary.h
    Source/Utils/UtilsFunctionLibrary.cpp
    Source/Utils/UtilsFunctionLibrary.h
)

# Adiciona o executável
add_executable(ClassicLauncher ${SOURCES})

# Configura bibliotecas e opções específicas do sistema operacional
if(WIN32)
    # Diretório da Raylib no Windows
    set(RAYLIB_DIR "C:/raylib-5.5_win64_msvc16")
    include_directories(${RAYLIB_DIR}/include)
    target_link_directories(ClassicLauncher PRIVATE ${RAYLIB_DIR}/lib)

    # Bibliotecas para Windows
    target_link_libraries(ClassicLauncher raylib opengl32 gdi32 winmm)
else()
    # Bibliotecas para Linux
    target_link_libraries(ClassicLauncher raylib GL m pthread dl rt vlc)
endif()
