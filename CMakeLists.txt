cmake_minimum_required(VERSION 3.25)
project(ClassicLauncher LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Cache options default values
option(CLASSIC_LAUNCHER_FSANITIZE "Enable address sanitizer" OFF)
option(CLASSIC_LAUNCHER_FLAGS "Enable warning/error flags" OFF)
option(CLASSIC_RASPBERRY_PI "Enable Raspberry Pi configuration" OFF)

# Release optimization
if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /O2")
else()
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3")
endif()


# Setting parameters for raylib
set(BUILD_EXAMPLES     OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES        OFF CACHE BOOL "" FORCE)
set(PLATFORM           SDL CACHE STRING "" FORCE)
#set(OPENGL_VERSION     4.3 CACHE STRING "" FORCE)
set(GLFW_BUILD_WAYLAND ON  CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11     OFF CACHE BOOL "" FORCE)

if(CLASSIC_RASPBERRY_PI)
    set(GRAPHICS GRAPHICS_API_OPENGL_ES2 CACHE STRING "" FORCE)
endif()

# SDL2 setup
if("${PLATFORM}" MATCHES "SDL")
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
    add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
endif()

if(CLASSIC_LAUNCHER_FSANITIZE)
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

message(STATUS "[Classic Launcher] Source dir is: ${CMAKE_SOURCE_DIR}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(debug "_DEBUG")
    message(STATUS "[Classic Launcher] DevMode is enabled ${debug}")
else()
    message(STATUS "[Classic Launcher] DevMode is disabled")
endif()

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

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
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE
            "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mwindows -e mainCRTStartup")
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

    list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules
    )

    find_package(VLC REQUIRED)

    if(VLC_FOUND)
        message(STATUS "[Classic Launcher] VLC found!")
        message(STATUS "[Classic Launcher] VLC Libraries: ${VLC_LIBRARIES}")
        message(STATUS "[Classic Launcher] Include Directory: ${VLC_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "[Classic Launcher] VLC not found!")
    endif()

    set(VLC_INCLUDE_DIRS ${VLC_INCLUDE_DIR})
    set(VLC_LINK_LIBS ${VLC_LIBRARIES})

    set(VLC_DLLS
        ${EXTERN_LIBDIR}/libvlc/x64/libvlc.dll
        ${EXTERN_LIBDIR}/libvlc/x64/libvlccore.dll
    )

    set(VLC_PLUGINS_DIR ${EXTERN_LIBDIR}/libvlc/x64/plugins)

elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(VLC REQUIRED libvlc)

    message(STATUS "[Classic Launcher] VLC include dirs: ${VLC_INCLUDE_DIRS}")
    message(STATUS "[Classic Launcher] VLC libraries: ${VLC_LIBRARIES}")

    set(VLC_LINK_LIBS ${VLC_LIBRARIES})
endif()

include_directories(
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/Source/Extern"
)

add_executable(ClassicLauncher)
include(${CMAKE_SOURCE_DIR}/CMake/ListSources.cmake)

# Submodule raylib
add_subdirectory(extern/raylib)

# Target-specific include dirs
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/Source/Extern"
    ${VLC_INCLUDE_DIRS}
)

# Warning / dev flags
if(CLASSIC_LAUNCHER_FLAGS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /EHsc /W3)
        message(STATUS "[Classic Launcher] Added flags /EHsc /W3")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
        message(STATUS "[Classic Launcher] Added flags -Wall -Wextra -Wpedantic")
    endif()
endif()

if(WIN32)
    message(STATUS "[Classic Launcher] Windows activated")

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CONSOLE
        ssize_t=intptr_t
        ${debug}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        ${VLC_LINK_LIBS}
    )

elseif(CLASSIC_RASPBERRY_PI)
    message(STATUS "[Classic Launcher] Raspberry Pi native activated")

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ${debug}
        PLATFORM_RPI
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        m
        atomic
        pthread
        GLESv2
        EGL
        gbm
        drm
        ${VLC_LINK_LIBS}
    )

else() # Linux
    message(STATUS "[Classic Launcher] Linux activated")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ${debug}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        m
        pthread
        GL
        dl
        rt
        ${VLC_LINK_LIBS}
    )
endif()

# extras post command post build
if(WIN32)
    foreach(DLL ${VLC_DLLS})
        add_custom_command(TARGET ClassicLauncher POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:ClassicLauncher>
        )
    endforeach()

    add_custom_command(TARGET ClassicLauncher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${VLC_PLUGINS_DIR}
        $<TARGET_FILE_DIR:ClassicLauncher>/plugins
    )
endif()

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
    COMMENT "Update compile_commands.json in \"${CMAKE_SOURCE_DIR}\""
)

# Build examples:
# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .

# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .

# Raspberry Pi:
# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release -DCLASSIC_RASPBERRY_PI=ON ..
# cmake --build . -j4