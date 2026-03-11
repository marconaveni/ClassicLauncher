cmake_minimum_required(VERSION 3.25)
project(ClassicLauncher LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# cache options default values
option(CLASSIC_LAUNCHER_FSANITIZE "Enable address sanitizer" OFF)
option(CLASSIC_LAUNCHER_FLAGS "Enable warning/error flags" OFF)
option(CLASSIC_RASPBERRY_PI "Enable Raspberry pi configuration" OFF)


# Setting parameters for raylib
set(BUILD_EXAMPLES     OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES        OFF CACHE BOOL "" FORCE) # or games
set(PLATFORM           SDL CACHE STRING "" FORCE) # use SDL backend instead of GLFW    
#set(OPENGL_VERSION     4.3 CACHE STRING "" FORCE) # change OpenGL version    
set(GLFW_BUILD_WAYLAND ON  CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11     OFF CACHE BOOL "" FORCE)

if (CLASSIC_RASPBERRY_PI)
    set(GRAPHICS GRAPHICS_API_OPENGL_ES2 CACHE STRING "" FORCE)
endif()


# SDL2 setup
if ("${PLATFORM}" MATCHES "SDL")

    set(SDL_SHARED ON CACHE BOOL "" FORCE) 
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)

    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
    add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
endif()


if(CLASSIC_LAUNCHER_FLAGS)
    if(MSVC)
        target_compile_options(ClassicLauncher PRIVATE /EHsc /W3)
        message(STATUS "[Classic Launcher] Added flags /EHsc /W3")
    else()
       target_compile_options(ClassicLauncher PRIVATE -Wall -Wextra -Wpedantic)
        message(STATUS "[Classic Launcher] Added flags -Wall -Wextra -Wpedantic")
    endif()
endif()

if (CLASSIC_LAUNCHER_FSANITIZE)
    if(MSVC)
        string(APPEND CMAKE_CXX_FLAGS_DEBUG " /MDd /fsanitize=address")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " /fsanitize=address")
    else()
        string(APPEND CMAKE_CXX_FLAGS " -g -fsanitize=address")
        string(APPEND CMAKE_LINKER_FLAGS " -fsanitize=address")
    endif()
    message(STATUS "[Classic Launcher] fsanitize is enabled")
else()
    message(STATUS "[Classic Launcher] fsanitize is disabled")
endif()

if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /O2")
else()
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O2")
endif()



message(STATUS "[Classic Launcher] Source dir is: " ${CMAKE_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(debug "_DEBUG")
    message(STATUS "[Classic Launcher] DevMode is enabled ${debug}")
else()    
    message(STATUS "[Classic Launcher] DevMode is disable")
endif()


if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ENTRY:mainCRTStartup  /SUBSYSTEM:WINDOWS")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/MTd)
        message(STATUS "[Classic Launcher] set /MTd")
    else()    
        add_compile_options(/MT)
        message(STATUS "[Classic Launcher] set /MT")
    endif()
endif()

if(MINGW)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")     
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    else()    
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mwindows -e mainCRTStartup")
    endif()
endif()

################## setup libvlc windows #####################
if(WIN32)  
    
    set(EXTERN_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/extern)
    set(
        CMAKE_PREFIX_PATH
        ${EXTERN_LIBDIR}/libvlc
        ${EXTERN_LIBDIR}/libvlc/x64
    )

    LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules
    )

    # Try to find the VLC library
    find_package(VLC REQUIRED)

    if(VLC_FOUND)
        message(STATUS "[Classic Launcher] VLC founded!")
        message(STATUS "[Classic Launcher] VLC Libraries: ${VLC_LIBRARIES}")
        message(STATUS "[Classic Launcher] Include Directory: ${VLC_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "[Classic Launcher] VLC not founded!")
    endif()

    # Includes the directories found.
    include_directories(${VLC_INCLUDE_DIR})

    # Path to the necessary files
    set(VLC_DLLS
    ${EXTERN_LIBDIR}/libvlc/x64/libvlc.dll
    ${EXTERN_LIBDIR}/libvlc/x64/libvlccore.dll
    )
    # plugins folder
    set(VLC_PLUGINS_DIR ${EXTERN_LIBDIR}/libvlc/x64/plugins)

endif() # WIN32

include_directories(   # Add header files
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/Source/Extern"
)

add_executable(ClassicLauncher) # start ClassicLauncher 
include(${CMAKE_SOURCE_DIR}/CMake/ListSources.cmake) # Add Source Files

# Submodule Raylib
add_subdirectory(extern/raylib)

if(WIN32)

    # Preprocessors definitions
    target_compile_definitions(ClassicLauncher PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CONSOLE
        ssize_t=intptr_t   #necessary for vlc
        ${debug}
    )

    # Windows
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        raylib 
        ${VLC_LIBRARIES}
    )


elseif (CLASSIC_RASPBERRY_PI)
    message(STATUS "[Classic Launcher] Raspberry pi native activated")

    # Preprocessors definitions
    target_compile_definitions(ClassicLauncher PRIVATE 
        ${debug}
    )

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

else() # Linux

    target_compile_definitions(ClassicLauncher PRIVATE     
        ${debug}
    )

    # Linux
    target_link_libraries(${PROJECT_NAME} 
        raylib 
        m 
        pthread 
        GL 
        dl 
        rt 
        vlc
    )
endif()



# extras post command post build

if(WIN32)

    # Rule for copying libvlc DLLs to the build directory.
    foreach(DLL ${VLC_DLLS})
    add_custom_command(TARGET ClassicLauncher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL}
        $<TARGET_FILE_DIR:ClassicLauncher>
    )
    endforeach()

    # Rule for copying the libvlc plugins folder to the build directory.
    add_custom_command(TARGET ClassicLauncher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${VLC_PLUGINS_DIR}
        $<TARGET_FILE_DIR:ClassicLauncher>/plugins
    )

endif()

# Rule for copying the resources folder to the build directory.
add_custom_command(TARGET ClassicLauncher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:ClassicLauncher>/Resources
)


add_custom_target(copy_compile_commands ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json
  BYPRODUCTS ${CMAKE_SOURCE_DIR}/compile_commands.json
  COMMENT "Update compile_commands.json in  \"${CMAKE_SOURCE_DIR}\" "
)







# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .

# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .

