cmake_minimum_required(VERSION 3.10)

project(HelloWindow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# GLFW
# ============================================================

find_package(glfw3 3.3 QUIET)

if (NOT glfw3_FOUND)

    if (EXISTS "${CMAKE_SOURCE_DIR}/libs/glfw/CMakeLists.txt")

        message(STATUS "Using bundled GLFW source in libs/glfw")

        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

        add_subdirectory(libs/glfw)

    else()

        message(FATAL_ERROR
            "GLFW not found."
        )

    endif()

endif()


# ============================================================
# GLAD
# ============================================================

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c")

    message(FATAL_ERROR
        "GLAD sources not found in libs/glad."
    )

endif()

add_library(glad STATIC
    libs/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/libs/glad/include
)


# ============================================================
# Assimp
# ============================================================

if (EXISTS "${CMAKE_SOURCE_DIR}/libs/assimp/CMakeLists.txt")

    message(STATUS "Using bundled Assimp source in libs/assimp")

    # Don't build unnecessary Assimp components
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)

    add_subdirectory(libs/assimp)

else()

    message(FATAL_ERROR
        "Assimp sources not found in libs/assimp."
    )

endif()


# ============================================================
# Main executable
# ============================================================

add_executable(HelloWindow

    src/main.cpp

    # Engine
    src/EngineFiles/Engine/Entity.cpp
    src/EngineFiles/Engine/StorageManager.cpp
    src/EngineFiles/RenderStuff/Model.cpp
    src/EngineFiles/RenderStuff/Texture.cpp
    src/EngineFiles/RenderStuff/Rig.cpp
    src/EngineFiles/Engine/EngineModelLoader.cpp
    src/EngineFiles/Engine/EngineTextureLoader.cpp
    src/EngineFiles/Engine/EngineBiomeLoader.cpp
    src/EngineFiles/RenderStuff/Shader.cpp
    src/EngineFiles/RenderStuff/Animator.cpp
    src/EngineFiles/RenderStuff/RenderSystem.cpp
    src/EngineFiles/Engine/TransformSystem.cpp
    src/EngineFiles/PhysicsStuff/AABBSystem.cpp
    src/EngineFiles/PhysicsStuff/PhysicsSystem.cpp
    src/EngineFiles/Engine/CameraSystem.cpp
    src/EngineFiles/Engine/PlayerMovementSystem.cpp
    src/EngineFiles/Engine/ChunkSystem.cpp
    src/EngineFiles/RenderStuff/ChunkRenderer.cpp
    src/EngineFiles/Engine/WorldGenerator.cpp
    src/EngineFiles/Engine/EngineDebug.cpp

    libs/fastnoise/FastNoise.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")

    target_compile_options(HelloWindow PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(HelloWindow PRIVATE
        -fsanitize=address,undefined
    )

endif()


# ============================================================
# Include directories
# ============================================================

target_include_directories(HelloWindow PRIVATE

    # GLAD
    ${CMAKE_SOURCE_DIR}/libs/glad/include

    # Assimp
    ${CMAKE_SOURCE_DIR}/libs/assimp/include

    # Other libraries
    ${CMAKE_SOURCE_DIR}/libs

    # Your headers
    ${CMAKE_SOURCE_DIR}/src/HeaderFiles
)


# ============================================================
# Libraries
# ============================================================

target_link_libraries(HelloWindow PRIVATE

    glfw

    glad

    assimp

    ${CMAKE_DL_LIBS}
)


# ============================================================
# OpenGL
# ============================================================

if (APPLE)

    target_link_libraries(HelloWindow PRIVATE
        "-framework OpenGL"
    )

elseif (UNIX AND NOT APPLE)

    find_package(OpenGL REQUIRED)

    target_link_libraries(HelloWindow PRIVATE
        OpenGL::GL
    )

elseif (WIN32)

    target_link_libraries(HelloWindow PRIVATE
        opengl32
    )

endif()