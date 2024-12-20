cmake_minimum_required(VERSION 3.28)
project(ClassicLauncher LANGUAGES CXX)

# Compile configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cache options default values
option(CLASSIC_LAUCHER_FSANITIZE "Enable address sanitizer" OFF)
option(CLASSIC_LAUCHER_FLAGS "Enable warning/error flags" OFF)

if(CLASSIC_LAUCHER_FLAGS)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
endif()

if (CLASSIC_LAUCHER_FSANITIZE)
    message("fsanitize is enabled")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")
else()
    message("fsanitize is disabled")
    # set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2 ")

# Add header files
include_directories(
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/extern
)

message("is source dir is: " ${CMAKE_SOURCE_DIR})

set(data
    "${CMAKE_SOURCE_DIR}/Source/Data/DateTime.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Data/DateTime.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/GameListManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Data/GameListManager.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/Vector2.h"
)
set(audio
    "${CMAKE_SOURCE_DIR}/Source/Audio/AudioManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Audio/AudioManager.h"
)
set(graphics
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Render.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Render.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Sprite.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Sprite.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/SpriteManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/SpriteManager.h"
)
set(guis
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiWindow.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiWindow.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiCard.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiCard.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiHorizontalBox.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiHorizontalBox.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiMiniCover.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiMiniCover.h"
)
set(entity
    "${CMAKE_SOURCE_DIR}/Source/Entity/Entity.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Entity/Entity.h"
    "${CMAKE_SOURCE_DIR}/Source/Entity/EntityManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Entity/EntityManager.h"
)
set(utils
    "${CMAKE_SOURCE_DIR}/Source/Utils/Math.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Print.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Print.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Resources.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/StringFunctionLibrary.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/StringFunctionLibrary.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/UtilsFunctionLibrary.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/UtilsFunctionLibrary.h"
)
set(src
    "${CMAKE_SOURCE_DIR}/Source/Core.h"
    "${CMAKE_SOURCE_DIR}/Source/Main.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Application.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Application.h"
    "${CMAKE_SOURCE_DIR}/Source/GuiComponent.cpp"
    "${CMAKE_SOURCE_DIR}/Source/GuiComponent.h"
)

set(extern 
    "${CMAKE_SOURCE_DIR}/extern/tinyxml2/tinyxml2.cpp"
    "${CMAKE_SOURCE_DIR}/extern/tinyxml2/tinyxml2.h"
)

source_group("Source\\Audio"     FILES ${audio})
source_group("Source\\Data"     FILES ${data})
source_group("Source\\Entity"     FILES ${entity})
source_group("Source\\Graphics"  FILES ${graphics})
source_group("Source\\Guis"  FILES ${guis})
source_group("Source\\Utils"     FILES ${utils})
source_group("Source"            FILES ${src})
source_group("Extern"     FILES ${extern})

# List of source files
set(srcs   
    ${audio}
    ${data}  
    ${graphics}
    ${guis}
    ${entity}
    ${utils}
    ${src}  
    ${extern}
)

# Add the executable
add_executable(ClassicLauncher ${srcs})


if(WIN32)

    # Setting parameters for raylib
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

    # Submodule Raylib 
    add_subdirectory(extern/raylib)

    # Preprocessors definitions
    target_compile_definitions(ClassicLauncher PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CONSOLE
    ssize_t=intptr_t
    )

    # Windows
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
else()

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    # Submodule Raylib
    add_subdirectory(extern/raylib)

    # Add `_DEBUG` only Debug build
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(ClassicLauncher PRIVATE _DEBUG)
    endif()

    # Linux
    target_link_libraries(${PROJECT_NAME} raylib GL m pthread dl rt)
endif()


# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .