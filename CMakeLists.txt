cmake_minimum_required(VERSION 3.25)
project(ClassicLauncher LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)


# Cache options default values
option(CLASSIC_LAUNCHER_FLAGS "Enable warning flags" OFF)
option(CLASSIC_RASPBERRY_PI "Enable Raspberry Pi configuration" OFF)
option(CLASSIC_SDL_INPUT "Enable SDL input only" ON)
option(CLASSIC_SDL_RAYLIB "Enables SDL backend controlled by Raylib." ON)


# Setting parameters for raylib
set(BUILD_EXAMPLES     OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES        OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND ON  CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11     OFF CACHE BOOL "" FORCE)
#set(OPENGL_VERSION     4.3 CACHE STRING "" FORCE)

if(CLASSIC_RASPBERRY_PI)
    set(GRAPHICS GRAPHICS_API_OPENGL_ES2 CACHE STRING "" FORCE)
endif()

# optimize
if(MSVC)
    string(APPEND CMAKE_C_FLAGS_RELEASE " /O2")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /O2")
else()
    string(APPEND CMAKE_C_FLAGS_RELEASE " -O3")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3")
endif()

# SDL2 setup
set(CLASSIC_SDL_LIB "") # Clear the cache just to be safe.
set(SDL_GAMEPAD "")
if(CLASSIC_SDL_INPUT OR CLASSIC_SDL_RAYLIB OR NOT WIN32) # Note: In Linux, we always use SDL.

    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
    add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)  # here added SDL submodule folder

    if(CLASSIC_SDL_RAYLIB)
        set(PLATFORM   SDL CACHE STRING "" FORCE) 
        message(STATUS "[Classic Launcher] Using SDL Backend.")
    else()
        set(CLASSIC_SDL_LIB  SDL3::SDL3)
        set(SDL_GAMEPAD "SDL_GAMEPAD")
        message(STATUS "[Classic Launcher] Using SDL Input only.")
    endif()

else()
    message(STATUS "[Classic Launcher] SDL is Disabled.")
endif()


message(STATUS "[Classic Launcher] Source dir is: ${CMAKE_SOURCE_DIR}")


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

else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(VLC REQUIRED libvlc)

    message(STATUS "[Classic Launcher] VLC include dirs: ${VLC_INCLUDE_DIRS}")
    message(STATUS "[Classic Launcher] VLC libraries: ${VLC_LIBRARIES}")

    set(VLC_LINK_LIBS ${VLC_LIBRARIES})
endif()


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

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4> /EHsc)
    
endif()


target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
)

if(CLASSIC_RASPBERRY_PI)
    message(STATUS "[Classic Launcher] Raspberry Pi native activated")

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        PLATFORM_RPI
    )

    set(UNCOMMON_LIBS 
        m
        atomic
        pthread
        GLESv2
        EGL
        gbm
        drm
    )

elseif(WIN32)
    
    message(STATUS "[Classic Launcher] Windows activated")

    if(MSVC)
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS>
        )
    elseif(MINGW)
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:-mwindows -e mainCRTStartup>
        )
    endif()
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        ${SDL_GAMEPAD}
    )
    target_compile_definitions(raylib PRIVATE 
        _CRT_SECURE_NO_WARNINGS
    )

    set(UNCOMMON_LIBS "")

else()
    message(STATUS "[Classic Launcher] Linux activated")

    set(UNCOMMON_LIBS 
        m
        pthread
        GL
        dl
        rt
    )

endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    ${UNCOMMON_LIBS}
    ${CLASSIC_SDL_LIB}
    ${VLC_LINK_LIBS}
)

# extras post command post build
if(WIN32)
    foreach(DLL ${VLC_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${VLC_PLUGINS_DIR}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)

if(NOT MSVC)
    add_custom_target(copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
        BYPRODUCTS ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Update compile_commands.json in \"${CMAKE_SOURCE_DIR}\""
    )
endif()

# Tests

add_executable(classiclauncher_tests
    Test/main.cpp
    Test/test.cpp
    Source/Guis/GuiBase.cpp
    Source/Entity/Entity.cpp
)
target_include_directories(classiclauncher_tests PRIVATE Source)
target_compile_features(classiclauncher_tests PRIVATE cxx_std_20)

enable_testing()

add_test(NAME classiclauncher_tests COMMAND $<TARGET_FILE:classiclauncher_tests>)




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