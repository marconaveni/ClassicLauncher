

file(GLOB animations_sources "Source/Animations/*.cpp")
file(GLOB animations_headers "Source/Animations/*.h")

file(GLOB audio_sources "Source/Audio/*.cpp")
file(GLOB audio_headers "Source/Audio/*.h")

file(GLOB components_sources "Source/Components/*.cpp")
file(GLOB components_headers "Source/Components/*.h")

file(GLOB data_sources "Source/Data/*.cpp")
file(GLOB data_headers "Source/Data/*.h")

file(GLOB entity_sources "Source/Entity/*.cpp")
file(GLOB entity_headers "Source/Entity/*.h")

file(GLOB graphics_sources "Source/Graphics/*.cpp")
file(GLOB graphics_headers "Source/Graphics/*.h")

file(GLOB guis_sources "Source/Guis/*.cpp")
file(GLOB guis_headers "Source/Guis/*.h")

file(GLOB input_sources "Source/Input/*.cpp")
file(GLOB input_headers "Source/Input/*.h")

file(GLOB themes_sources "Source/Themes/*.cpp")
file(GLOB themes_headers "Source/Themes/*.h")

file(GLOB utils_sources "Source/Utils/*.cpp")
file(GLOB utils_headers "Source/Utils/*.h")

file(GLOB video_sources "Source/Video/*.cpp")
file(GLOB video_headers "Source/Video/*.h")

file(GLOB window_sources "Source/Window/*.cpp")
file(GLOB window_headers "Source/Window/*.h")

file(GLOB_RECURSE extern_sources "Source/Extern/*.cpp")
file(GLOB_RECURSE extern_headers "Source/Extern/*.h")

file(GLOB src_sources "Source/*.cpp")
file(GLOB src_headers "Source/*.h")



# Group for better Visual in IDE
source_group("Source\\Animations" FILES ${animations_sources})
source_group("Source\\Audio"      FILES ${audio_sources})
source_group("Source\\Components" FILES ${components_sources})
source_group("Source\\Data"       FILES ${data_sources})
source_group("Source\\Entity"     FILES ${entity_sources})
source_group("Source\\Graphics"   FILES ${graphics_sources})
source_group("Source\\Guis"       FILES ${guis_sources})
source_group("Source\\Input"      FILES ${input_sources})
source_group("Source\\Themes"     FILES ${themes_sources})
source_group("Source\\Utils"      FILES ${utils_sources})
source_group("Source\\Video"      FILES ${video_sources})
source_group("Source\\Window"     FILES ${window_sources})
source_group("Source\\Extern"     FILES ${extern_sources})
source_group("Source"             FILES ${src_sources})

source_group("Headers\\Animations" FILES ${animations_headers})
source_group("Headers\\Audio"      FILES ${audio_headers})
source_group("Headers\\Components" FILES ${components_headers})
source_group("Headers\\Data"       FILES ${data_headers})
source_group("Headers\\Entity"     FILES ${entity_headers})
source_group("Headers\\Graphics"   FILES ${graphics_headers})
source_group("Headers\\Guis"       FILES ${guis_headers})
source_group("Headers\\Input"      FILES ${input_headers})
source_group("Headers\\Themes"     FILES ${themes_headers})
source_group("Headers\\Utils"      FILES ${utils_headers})
source_group("Headers\\Video"      FILES ${video_headers})
source_group("Headers\\Window"     FILES ${window_headers})
source_group("Headers\\Extern"     FILES ${extern_headers}) 
source_group("Headers"             FILES ${src_headers})

message(STATUS ${extern_headers})

# Add all *.cpp files
set(srcs
    ${animations_sources}
    ${audio_sources}
    ${components_sources}
    ${data_sources}
    ${entity_sources}
    ${graphics_sources}
    ${guis_sources}
    ${input_sources}
    ${themes_sources}
    ${utils_sources}
    ${video_sources}
    ${window_sources}
    ${src_sources}
    ${extern_sources}
)

# Add all *.h files
set(headers
    ${animations_headers}
    ${audio_headers}
    ${components_headers}
    ${data_headers}
    ${entity_headers}
    ${graphics_headers}
    ${guis_headers}
    ${input_headers}
    ${themes_headers}
    ${utils_headers}
    ${video_headers}
    ${window_headers}
    ${src_headers}
    ${extern_headers} 
)

if(WIN32)
    set(rc
        "${CMAKE_SOURCE_DIR}/Source/ClassicLauncher.rc"
    )
    source_group("Resource" FILES ${rc})
endif()

# Adiciona ao alvo
target_sources(ClassicLauncher PRIVATE ${srcs} ${headers} ${rc})






