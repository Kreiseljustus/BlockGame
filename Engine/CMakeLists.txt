# ---------------------------------------------------------------------------
# Engine -- static library. Everything (glfw, glew, glm, assimp, lua, sol2,
# OpenGL) is linked PUBLIC, so any target that links against Engine alone
# transitively gets every dependency, every include path, and every compile
# definition it needs. Nothing outside this file should ever need to know
# these dependencies exist.
# ---------------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_library(Engine STATIC ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Required since we're statically linking GLEW (glew-cmake_BUILD_STATIC=ON
# in the top-level CMakeLists.txt) -- without this define, GLEW's headers
# assume dynamic linking, which can cause inconsistent/garbage state at
# runtime (e.g. glewInit() returning an unrecognized error code instead of
# a real GLEW_* error). PUBLIC so it's also defined for Sandbox/Editor,
# since they include GLEW-using engine headers.
target_compile_definitions(Engine PUBLIC GLEW_STATIC)

target_link_libraries(Engine PUBLIC
        glfw
        libglew_static
        glm::glm
        stb
        lua::lua
        sol2::sol2
        assimp::assimp
)

if(WIN32)
    target_link_libraries(Engine PUBLIC opengl32)
elseif(APPLE)
    target_link_libraries(Engine PUBLIC "-framework OpenGL")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(Engine PUBLIC OpenGL::GL)
endif()
