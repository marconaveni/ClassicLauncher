cmake_minimum_required(VERSION 3.25)
project(ClassicLauncher LANGUAGES CXX)

# Compile configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cache options default values
option(CLASSIC_LAUNCHER_FSANITIZE "Enable address sanitizer" OFF)
option(CLASSIC_LAUNCHER_FLAGS "Enable warning/error flags" OFF)
option(CLASSIC_RASPBERRY_PI "Enable Raspberry pi configuration" OFF)
option(CLASSIC_FORCE_THEME_1X "Force Theme 1x" OFF)

if(CLASSIC_FORCE_THEME_1X)
    message(STATUS "[Classic Launcher] FORCE_THEME_1X is enabled")
    set(forcetheme "FORCE_THEME_1X")
else()
    message(STATUS "[Classic Launcher] FORCE_THEME_1X is disabled")
endif()

if(CLASSIC_LAUNCHER_FLAGS)
    if(MSVC)
        set(CMAKE_CXX_FLAGS " /EHsc /W3")
        message(STATUS "[Classic Launcher] Added flags /EHsc /W3")
    else()
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
        message(STATUS "[Classic Launcher] Added flags -Wall -Wextra -Wpedantic")
    endif()
endif()

if (CLASSIC_LAUNCHER_FSANITIZE)
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "/MDd /fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/fsanitize=address")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
    endif()
    message(STATUS "[Classic Launcher] fsanitize is enabled")
else()
    message(STATUS "[Classic Launcher] fsanitize is disabled")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2 ")


# Add header files
include_directories(
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/extern
)

message(STATUS "[Classic Launcher] Source dir is: " ${CMAKE_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "[Classic Launcher] DevMode is enabled")
else()    
    message(STATUS "[Classic Launcher] DevMode is disable")
endif()


if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ENTRY:mainCRTStartup  /SUBSYSTEM:WINDOWS")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/MTd)
        message(STATUS "[Classic Launcher] set /MTd")
        set(debug "_DEBUG")
    else()    
        add_compile_options(/MT)
        message(STATUS "[Classic Launcher] set /MT")
    endif()
endif()

if(MINGW)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(debug "_DEBUG")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    else()    
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mwindows -e mainCRTStartup")
    endif()
endif()


if(WIN32)  
    
    # setup libvlc windows

    set(EXTERN_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/extern)
    set(
        CMAKE_PREFIX_PATH
        ${EXTERN_LIBDIR}/libvlc
        ${EXTERN_LIBDIR}/libvlc/x64
    )

    LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules
    )

    # Tente encontrar a biblioteca VLC
    find_package(VLC REQUIRED)

    if(VLC_FOUND)
        message(STATUS "[Classic Launcher] VLC founded!")
        message(STATUS "[Classic Launcher] VLC Libraries: ${VLC_LIBRARIES}")
        message(STATUS "[Classic Launcher] Include Directory: ${VLC_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "[Classic Launcher] VLC not founded!")
    endif()

    # Inclui os diretórios encontrados
    include_directories(${VLC_INCLUDE_DIR})

    # Caminho para os arquivos necessários
    set(VLC_DLLS
    ${EXTERN_LIBDIR}/libvlc/x64/libvlc.dll
    ${EXTERN_LIBDIR}/libvlc/x64/libvlccore.dll
    )
    # plugins folder
    set(VLC_PLUGINS_DIR ${EXTERN_LIBDIR}/libvlc/x64/plugins)

endif() # WIN32


# Source files
set(animations
    "${CMAKE_SOURCE_DIR}/Source/Animations/Animation.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Animations/Animation.h"
)
set(audio
    "${CMAKE_SOURCE_DIR}/Source/Audio/AudioManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Audio/AudioManager.h"
)
set(data
    "${CMAKE_SOURCE_DIR}/Source/Data/Color.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/DateTime.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Data/DateTime.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/GameListManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Data/GameListManager.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/Rectangle.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/Size.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/Vector2.h"
    "${CMAKE_SOURCE_DIR}/Source/Data/TransformProperties.h"
)
set(graphics
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Render.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Render.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Sprite.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/Sprite.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/SpriteAnimator.h"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/SpriteManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/SpriteManager.h"
)
set(guis
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiBlackScreen.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiBlackScreen.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiCard.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiCard.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiHorizontalBox.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiHorizontalBox.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiHorizontalCards.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiHorizontalCards.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiMiniCover.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiMiniCover.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiSizeBox.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiSizeBox.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiTextBlock.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiTextBlock.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiVideoPlayer.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiVideoPlayer.h"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiWindow.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Guis/GuiWindow.h"
)
set(input
    "${CMAKE_SOURCE_DIR}/Source/Input/InputManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Input/InputManager.h"
)
set(themes
    "${CMAKE_SOURCE_DIR}/Source/Themes/Themes.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Themes/Themes.h"
)
set(entity
    "${CMAKE_SOURCE_DIR}/Source/Entity/Entity.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Entity/Entity.h"
    "${CMAKE_SOURCE_DIR}/Source/Entity/EntityManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Entity/EntityManager.h"
)
set(utils
    "${CMAKE_SOURCE_DIR}/Source/Utils/Log.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Log.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Math.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Print.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Print.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Process.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Process.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/ProcessManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/ProcessManager.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Resources.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Resources.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/StringFunctionLibrary.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/StringFunctionLibrary.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/ThreadManager.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Timer.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/Timer.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/TimerManager.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/TimerManager.h"
    "${CMAKE_SOURCE_DIR}/Source/Utils/UtilsFunctionLibrary.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Utils/UtilsFunctionLibrary.h"
    )
set(video     
    "${CMAKE_SOURCE_DIR}/Source/Video/VideoPlayer.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Video/VideoPlayer.h"
)
set(src
    "${CMAKE_SOURCE_DIR}/Source/Application.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Application.h"
    "${CMAKE_SOURCE_DIR}/Source/ClassicAssert.h"
    "${CMAKE_SOURCE_DIR}/Source/ClassicLauncher.h"
    "${CMAKE_SOURCE_DIR}/Source/Core.h"
    "${CMAKE_SOURCE_DIR}/Source/GuiComponent.cpp"
    "${CMAKE_SOURCE_DIR}/Source/GuiComponent.h"
    "${CMAKE_SOURCE_DIR}/Source/Main.cpp"
    )
    
set(extern 
    "${CMAKE_SOURCE_DIR}/extern/tinyxml2/tinyxml2.cpp"
    "${CMAKE_SOURCE_DIR}/extern/tinyxml2/tinyxml2.h"
    "${CMAKE_SOURCE_DIR}/extern/reasings/reasings.h"
)
    
if(MSVC)
    LIST(APPEND src
        "${CMAKE_SOURCE_DIR}/Source/ClassicLauncher.rc"
    )
endif()

source_group("Source\\Animations" FILES ${animations})
source_group("Source\\Audio"     FILES ${audio})
source_group("Source\\Data"     FILES ${data})
source_group("Source\\Entity"     FILES ${entity})
source_group("Source\\Graphics"  FILES ${graphics})
source_group("Source\\Guis"  FILES ${guis})
source_group("Source\\Input"  FILES ${input})
source_group("Source\\Themes"  FILES ${themes})
source_group("Source\\Utils"     FILES ${utils})
source_group("Source\\Video"     FILES ${video})
source_group("Source"            FILES ${src})
source_group("Extern"     FILES ${extern})

# List of source files
set(srcs   
    ${animations}
    ${audio}
    ${data}  
    ${entity}
    ${graphics}
    ${guis}
    ${input}
    ${themes}
    ${utils}
    ${video}
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
    ${debug}
    ${forcetheme}
    )

    # Windows
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib ${VLC_LIBRARIES})


    # Regra para copiar as DLLs da libvlc para o diretório de build
    foreach(DLL ${VLC_DLLS})
    add_custom_command(TARGET ClassicLauncher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL}
        $<TARGET_FILE_DIR:ClassicLauncher>
    )
    endforeach()

    # Regra para copiar a pasta de plugins da libvlc para o diretório de build
    add_custom_command(TARGET ClassicLauncher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${VLC_PLUGINS_DIR}
        $<TARGET_FILE_DIR:ClassicLauncher>/plugins
    )

elseif (CLASSIC_RASPBERRY_PI)
    message(STATUS "[Classic Launcher] Raspberry pi native activated")

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(PLATFORM "DRM" CACHE STRING "Define Raylib platform as DRM")
    add_definitions(-DPLATFORM_DRM)

    # Submodule Raylib
    add_subdirectory(extern/raylib)

    # Definições específicas para Debug
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(ClassicLauncher PRIVATE _DEBUG ${FORCE_THEME_1X})
    endif()

    # -lraylib -lm -lpthread -lGLESv2 -lEGL -lvcos -lvchiq_arm -lgbm -ldrm
    # Link for Raspberry Pi
    target_link_libraries(${PROJECT_NAME}
        raylib
        m
        pthread
        GLESv2
        EGL
        vcos
        vchiq_arm
        gbm
        drm
        vlc
)
else()

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    # Submodule Raylib
    add_subdirectory(extern/raylib)

    # Add `_DEBUG` only Debug build
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(ClassicLauncher PRIVATE _DEBUG ${FORCE_THEME_1X})
    endif()

    # Linux
    target_link_libraries(${PROJECT_NAME} raylib GL m pthread dl rt vlc)
endif()

add_custom_command(TARGET ClassicLauncher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:ClassicLauncher>/Resources
)

# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .

# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .