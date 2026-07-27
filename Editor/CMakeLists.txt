file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable(Editor ${EDITOR_SOURCES})

# Linking Engine alone pulls in glfw/glew/glm/assimp/lua/sol2/OpenGL and all
# their include paths transitively -- nothing else needs to be listed here.
target_link_libraries(Editor PRIVATE Engine)

# ---------------------------------------------------------------------------
# Copy assets/ next to the built executable after every build.
# CMAKE_SOURCE_DIR (not CMAKE_CURRENT_SOURCE_DIR) always points at the
# top-level project dir, so this finds the one shared assets/ folder
# regardless of this file living in Editor/.
# ---------------------------------------------------------------------------
add_custom_command(TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Editor>/assets
        COMMENT "Copying assets/ to output directory"
)
