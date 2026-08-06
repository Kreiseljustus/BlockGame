cmake_minimum_required(VERSION 3.30)
project(BlockGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# --- Assimp (model/scene import) ---
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        v5.4.3
)

# Trim the build: we only need import (not export), and only formats
# we actually use. Building every importer + exporter + tests/samples
# adds several minutes to a clean build for no benefit here.
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)   # avoid relying on a system zlib
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)    # we only load models, never write them

# Only enable the importers we actually need -- massively cuts build time.
# Add more ASSIMP_BUILD_*_IMPORTER lines below as needed.
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glew glm stb lua sol2 assimp)

# stb is header-only with no CMakeLists.txt of its own; expose it as an
# interface target so we can link against it like any other dependency.
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# ---------------------------------------------------------------------------
# Subprojects
# ---------------------------------------------------------------------------
# Engine must come first -- Sandbox and Editor both link against it.
add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(Editor)
