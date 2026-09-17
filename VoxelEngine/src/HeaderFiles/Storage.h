    #pragma once
    #include <unordered_map>
    #include "Entity.h"
    #include "Component.h"
    struct EntityHash {
        std::size_t operator()(const Entity& ent) const {
            return std::hash<int>{}(ent.GetID());
        }
    };
    
    struct Storage {
        std::unordered_map<int, Entity> entityStorage;
        std::unordered_map<int, OpenGLComponent> openGLStorage;
        std::unordered_map<int, MeshComponent> meshStorage;
        std::unordered_map<int, TransformComponent> transformStorage;
        std::unordered_map<int, RigComponent> rigStorage;
        std::unordered_map<int, AnimatorComponent> animatorStorage;
        std::unordered_map<int, AABBComponent> AABBStorage;
        std::unordered_map<int, PhysicsComponent> physicsStorage;
        std::unordered_map<int, MaterialComponent> materialStorage;
        std::unordered_map<int, CameraComponent> cameraStorage;
        std::unordered_map<int, PlayerMovement> playerMovementStorage;
        std::unordered_map<int, ChunkComponent> chunkStorage;
    };