cmake_minimum_required(VERSION 3.30)
project(BlockGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Output directories (keeps .exe in a predictable place across generators)
# ---------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# ---------------------------------------------------------------------------
# FetchContent setup
# ---------------------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# Some fetched dependencies (e.g. glew-cmake) ship a CMakeLists.txt with an
# outdated `cmake_minimum_required`, which newer CMake versions reject outright.
# This tells CMake to apply policies as if that old minimum were valid instead
# of hard-erroring, without affecting our own project's policy version.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

# --- GLFW (windowing / input) ---
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# --- GLEW (OpenGL function loading) ---
FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG        glew-cmake-2.2.0
)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)

# --- GLM (math) ---
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
)

# --- stb (stb_image.h - header only, fetched as a single file) ---
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG        master
)

# --- Lua (CMake-ified mirror, since upstream Lua ships no CMakeLists.txt) ---
FetchContent_Declare(
        lua
        GIT_REPOSITORY https://github.com/marovira/lua
        GIT_TAG        5.4.8
)

# --- sol2 (C++ <-> Lua binding library, header only) ---
FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG        v3.3.0
)

FetchContent_MakeAvailable(glfw glew glm stb lua sol2)

# stb is header-only with no CMakeLists.txt of its own; expose it as an
# interface target so we can link against it like any other dependency.
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
# NOTE: GLOB_RECURSE already picks up every .cpp/.c under src/, including
# Window.cpp. Do NOT also list individual source files in add_executable()
# below -- doing so adds them a second time, causing duplicate symbol
# definitions at link time. Headers (.h) don't need to be listed at all for
# compilation to work; CLion only wants them listed so they show up in the
# project tree, which isn't necessary here since GLOB_RECURSE only matches
# .cpp/.c anyway.
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.c
)

add_executable(BlockGame ${GAME_SOURCES})

target_include_directories(BlockGame PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Required since we're statically linking GLEW (glew-cmake_BUILD_STATIC=ON
# above) -- without this define, GLEW's headers assume dynamic linking,
# which can cause inconsistent/garbage state at runtime (e.g. glewInit()
# returning an unrecognized error code instead of a real GLEW_* error).
target_compile_definitions(BlockGame PRIVATE GLEW_STATIC)

target_link_libraries(BlockGame PRIVATE
        glfw
        libglew_static
        glm::glm
        stb
        lua::lua
        sol2::sol2
)

if(WIN32)
    target_link_libraries(BlockGame PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(BlockGame PRIVATE "-framework OpenGL")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(BlockGame PRIVATE OpenGL::GL)
endif()

# ---------------------------------------------------------------------------
# Copy assets/ next to the built executable after every build
# ---------------------------------------------------------------------------
add_custom_command(TARGET BlockGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:BlockGame>/assets
        COMMENT "Copying assets/ to output directory"
)