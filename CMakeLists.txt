# Set standards
cmake_minimum_required(VERSION 3.16)
project(Django C CXX)

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////////// PROJECT FILES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

file(GLOB_RECURSE MY_SOURCES src/*)
file(GLOB_RECURSE MY_INTERNAL_SHADERS internal-shaders/*)
file(GLOB_RECURSE MY_CONFIGS configurations/*)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////// DEPENDENCIES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

find_package(OpenGL REQUIRED)

# On windows
if (WIN32) 
    set(SDL2_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.10/include)
    set(SDL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2.lib;
                       ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2main.lib)

# On linux
else()
    find_package(SDL2 REQUIRED)
endif()

# ------------------------------ BUILD IF NEEDED ------------------------------

add_library(GLAD STATIC "lib/glad/src/glad.c")
add_library(STB_IMAGE STATIC "lib/stb_image/stb_image.cpp")
add_library(STB_IMAGE_WRITE STATIC "lib/stb_image/stb_image_write.cpp")
file(GLOB_RECURSE IMGUI_SOURCES lib/imgui/*)
add_library(IMGUI STATIC ${IMGUI_SOURCES})
add_subdirectory("lib/Boxer")
add_subdirectory("lib/nativefiledialog-extended/src")

# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// INCLUDES //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    lib/glad/include
    lib/imgui
    lib/glm
    lib/color/src
    lib/cereal-1.3.0/include/
    lib/nativefiledialog-extended/src/include
    lib
    src
    res
)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////////// BUILD ///////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
add_executable(${PROJECT_NAME} ${MY_SOURCES}
    lib/nanosvg/nanosvg.cpp # compile the nanosvg library
    "${CMAKE_SOURCE_DIR}/windows-app-ressources/django.rc" # add ressources for Windows (app icon)
    ${MY_INTERNAL_SHADERS} ${MY_CONFIGS} # include in the VS project so they will appear in the solution explorer
)
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    GLAD
    STB_IMAGE
    STB_IMAGE_WRITE
    IMGUI
    Boxer
    nfd
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Recreate folder architecture inside visual studio solution (might work for other IDEs as well)
include("Cool/CMake-Helpers/VS_helpers.cmake")
VS_RegisterFiles("${MY_SOURCES}")
VS_RegisterFiles("${MY_INTERNAL_SHADERS}")
VS_RegisterFiles("${MY_CONFIGS}")

if (UNIX)
    target_link_libraries(${PROJECT_NAME} -ldl)
endif()

# Copy .dll to build folder
if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
# Add a post build operation to copy folders to the output folder (where the executable is created)
include("Cool/CMake-Helpers/copy_folder_to_the_output_folder.cmake")
copy_folder_to_the_output_folder("internal-shaders")
copy_folder_to_the_output_folder("presets")
copy_folder_to_the_output_folder("configurations")
copy_folder_to_the_output_folder("icons")
# And copy imgui.ini
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/imgui.ini
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini
)

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////// PRECOMPILED HEADER ! /////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

target_precompile_headers(${PROJECT_NAME}
  PRIVATE
  # Libraries
    <imgui/imgui.h>
    <imgui/misc/cpp/imgui_stdlib.h>
    <glm/vec2.hpp>
    <glm/vec3.hpp>
    <glad/glad.h>
    <SDL2/SDL.h>
    <spdlog/spdlog.h>
    <cereal/access.hpp>
    <cereal/types/vector.hpp>
    <cereal/types/string.hpp>
    <cereal/types/memory.hpp>
  # std
    <vector>
    <string>
    <random>
    <memory>
    <unordered_map>
    <queue>
  # Project Code
    [["Debugging/Log.h"]]
    [["Debugging/glException.h"]]
    [["Helper/glmCereal.h"]]
)