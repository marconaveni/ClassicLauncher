cmake_minimum_required(VERSION 3.28)
project(ClassicLauncher LANGUAGES CXX)

# Configurações de compilação
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona diretórios de cabeçalhos
include_directories(
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/extern
)

set(graphics
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Render.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Render.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Sprite.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Sprite.cpp"
)
set(src
    "${CMAKE_SOURCE_DIR}/Source/Main.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Application.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Application.h"
)
set(audio
    "${CMAKE_SOURCE_DIR}/Source/Audio/AudioManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Audio/AudioManager.h"
)
set(utils
    "${CMAKE_SOURCE_DIR}/Source/Utils/Core.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Math.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Print.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Print.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Resouces.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/StringFunctionLibrary.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/StringFunctionLibrary.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/UtilsFunctionLibrary.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/UtilsFunctionLibrary.h"
)

source_group("Source"            FILES ${src})
source_group("Source\\Graphics"  FILES ${graphics})
source_group("Source\\Audio"     FILES ${audio})
source_group("Source\\Utils"     FILES ${utils})

# Lista de arquivos fonte
set(srcs   
    ${src}  
    ${graphics}
    ${audio}
    ${utils}
)

# Adiciona o executável
add_executable(ClassicLauncher ${srcs})


if(WIN32)

    # Setting parameters for raylib
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

    # Submodule Raylib 
    add_subdirectory(extern/raylib)

    # Pré-processadores para o alvo
    target_compile_definitions(ClassicLauncher PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CONSOLE
    ssize_t=intptr_t
    )

    # Windows
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
else()

    # Add `_DEBUG` only Debug build
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(ClassicLauncher PRIVATE _DEBUG)
    endif()

    # Linux
    target_link_libraries(${PROJECT_NAME} raylib GL m pthread dl rt vlc)
endif()


# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .