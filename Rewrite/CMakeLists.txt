cmake_minimum_required(VERSION 3.20)

project(A_Pirates_Tale LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW
add_subdirectory(libs/glfw)

# Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/assimp)

# GLAD
add_library(glad STATIC
    libs/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    libs/glad/include
)

# FastNoise
file(GLOB FASTNOISE_SOURCES
    "libs/fastnoise/*.cpp"
    "libs/fastnoise/*.c"
)

find_path(LUA_INCLUDE_DIR lua.h
    PATHS
        "${LUA_ROOT}/include/lua5.4"
        /opt/homebrew/opt/lua@5.4/include/lua5.4
    NO_DEFAULT_PATH
    REQUIRED
)

find_library(LUA_LIBRARY
    NAMES lua5.4 lua.5.4 lua
    PATHS
        "${LUA_ROOT}/lib"
        /opt/homebrew/opt/lua@5.4/lib
    NO_DEFAULT_PATH
    REQUIRED
)

if(FASTNOISE_SOURCES)
    add_library(fastnoise STATIC ${FASTNOISE_SOURCES})
    target_include_directories(fastnoise PUBLIC
        libs/fastnoise
    )
else()
    add_library(fastnoise INTERFACE)
    target_include_directories(fastnoise INTERFACE
        libs/fastnoise
    )
endif()

# Game executable
add_executable(A_Pirates_Tale
    src/main.cpp

    src/Engine/Entities/Entity.cpp
    src/Engine/Entities/ScriptStorage.cpp

    src/Engine/EngineBlocks/Renderer.cpp
    src/Engine/EngineBlocks/Mesh.cpp
    src/Engine/EngineBlocks/Camera.cpp
    src/Engine/EngineBlocks/AABB.cpp
    src/Engine/EngineBlocks/DebugRenderer.cpp
    src/Engine/EngineBlocks/TextureAtlasStorage.cpp
    src/Engine/EngineBlocks/Shader.cpp
    src/Engine/EngineBlocks/TextureAtlas.cpp
    src/Engine/EngineBlocks/ModelStorage.cpp
    src/Engine/EngineBlocks/Model.cpp

    src/Engine/WorldGeneration/Chunk.cpp
    src/Engine/WorldGeneration/WorldGenerator.cpp
    src/Engine/WorldGeneration/BiomeStorage.cpp
    src/Engine/WorldGeneration/BlockStorage.cpp
    src/Engine/WorldGeneration/BlockModelStorage.cpp
    src/Engine/WorldGeneration/BlockEvents.cpp
    src/Engine/WorldGeneration/StructureStorage.cpp
    src/Engine/WorldGeneration/TreeBuilder.cpp
    src/Engine/WorldGeneration/Biome.cpp
    src/Engine/WorldGeneration/Block.cpp
    src/Engine/WorldGeneration/BlockMesh.cpp
    src/Engine/WorldGeneration/TreeStructure.cpp

    src/Engine/EngineBlocks/Packages/CameraController.cpp


)

target_include_directories(A_Pirates_Tale PRIVATE
    libs
    libs/fastnoise
    src/Engine/EngineBlocks
    src/Engine/EngineBlocks/Packages
    src/Engine/WorldGeneration
    src/Engine/Entities
    ${LUA_INCLUDE_DIR}
)

target_link_libraries(A_Pirates_Tale PRIVATE
    glad
    glfw
    assimp
    fastnoise
    ${LUA_LIBRARY}
    )

if(APPLE)

    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)

    target_link_libraries(A_Pirates_Tale PRIVATE
        ${OPENGL_FRAMEWORK}
    )

    target_compile_definitions(A_Pirates_Tale PRIVATE
        GL_SILENCE_DEPRECATION
    )

elseif(WIN32)

    find_package(OpenGL REQUIRED)

    target_link_libraries(A_Pirates_Tale PRIVATE
        OpenGL::GL
    )

elseif(UNIX)

    find_package(OpenGL REQUIRED)

    target_link_libraries(A_Pirates_Tale PRIVATE
        OpenGL::GL
    )

endif()