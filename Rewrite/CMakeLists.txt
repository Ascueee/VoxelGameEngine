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

    src/Engine/CPPFiles/Entity.cpp
    src/Engine/CPPFiles/Chunk.cpp
    src/Engine/CPPFiles/ChunkRenderer.cpp
    src/Engine/CPPFiles/Renderer.cpp
    src/Engine/CPPFiles/WorldGenerator.cpp
    src/Engine/CPPFiles/BiomeStorage.cpp
    src/Engine/CPPFiles/TextureAtlasStorage.cpp
    src/Engine/CPPFiles/BlockStorage.cpp
    src/Engine/CPPFiles/BlockModelStorage.cpp
    src/Engine/CPPFiles/BlockEvents.cpp
    src/Engine/CPPFiles/Camera.cpp

    src/Assets/CPPFiles/Mesh.cpp
    src/Assets/CPPFiles/Shader.cpp
    src/Assets/CPPFiles/Biome.cpp
    src/Assets/CPPFiles/Block.cpp
    src/Assets/CPPFiles/TextureAtlas.cpp
    src/Assets/CPPFiles/BlockMesh.cpp
)

target_include_directories(A_Pirates_Tale PRIVATE
    libs
    libs/fastnoise
    src/Engine/HeaderFiles
    src/Assets/HeaderFiles
)

target_link_libraries(A_Pirates_Tale PRIVATE
    glad
    glfw
    assimp
    fastnoise
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