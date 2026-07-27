file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable(Sandbox ${SANDBOX_SOURCES})

# Linking Engine alone pulls in glfw/glew/glm/assimp/lua/sol2/OpenGL and all
# their include paths transitively -- nothing else needs to be listed here.
target_link_libraries(Sandbox PRIVATE Engine)

# ---------------------------------------------------------------------------
# Copy assets/ next to the built executable after every build.
# CMAKE_SOURCE_DIR (not CMAKE_CURRENT_SOURCE_DIR) always points at the
# top-level project dir, so this finds the one shared assets/ folder
# regardless of this file living in Sandbox/.
# ---------------------------------------------------------------------------
add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Sandbox>/assets
        COMMENT "Copying assets/ to output directory"
)
