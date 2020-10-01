# Set standards
cmake_minimum_required(VERSION 3.8)
project(DJANGO C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////////// PROJECT FILES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

################################ -------- ######
file(GLOB_RECURSE MY_SOURCES src/*)
file(GLOB_RECURSE MY_INTERNAL_SHADERS internal-shaders/*)
file(GLOB_RECURSE MY_CONFIGS configurations/*)

# Recreate folder architecture inside visual studio solution (might work for other IDEs as well)
# src
foreach(_source IN ITEMS ${MY_SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
# internal-shaders
foreach(_source IN ITEMS ${MY_INTERNAL_SHADERS})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
# configurations
foreach(_source IN ITEMS ${MY_CONFIGS})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
################################ -------- ######

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
    ${MY_INTERNAL_SHADERS} ${MY_CONFIGS} # include in the VS project so they will appear in the solution explorer
)
target_link_libraries(
    ${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    GLAD
    STB_IMAGE
    STB_IMAGE_WRITE
    IMGUI
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} -ldl)
endif()

if (WIN32) # Copy .dll to build folder
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Copy internal-shaders folder after a build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/internal-shaders/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/internal-shaders
)
# And copy settings folder
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/settings/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/settings
)
# And copy configurations folder
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/configurations/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/configurations
)
# And copy icons folder
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/icons/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/icons
)
# Setup working directory in Visual Studio (to access the res folder) :
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

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