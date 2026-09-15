#include "StorageManager.h"

Storage StorageManager::storage;
int StorageManager::entityCounter;

StorageManager::StorageManager(){}

void StorageManager::CreateBlankEntity(){
    Entity newEntity;
    TransformComponent transformComponent;
    newEntity.SetID(entityCounter);
    storage.entityStorage.emplace(entityCounter, newEntity);

    entityCounter++;
}

//using a model asset from the EngineModelLoader it builds the model into a entity heirarchy
void StorageManager::BuildModel(Model* model){
    auto& nodes = model->GetNodes();
    std::vector<int> nodeToEntityId(nodes.size());

    int entityRigHolder =0;
    for(size_t i = 0; i < nodes.size(); i++){
        Entity newEntity;    
        newEntity.SetID(entityCounter);
        newEntity.SetName(nodes[i].nodeName);

        int parentEntityId = -1;
        if(nodes[i].parentId != -1){
            parentEntityId = nodeToEntityId[nodes[i].parentId];
            newEntity.SetParentID(parentEntityId);
        }

        if(!nodes[i].modelMesh.vertices.empty()){
            OpenGLComponent openGLComponent;
            MaterialComponent materialComponent;
            nodes[i].modelMesh.rigRef = entityRigHolder;
            storage.openGLStorage.emplace(entityCounter, openGLComponent);
            storage.meshStorage.emplace(entityCounter, nodes[i].modelMesh);
            storage.materialStorage.emplace(entityCounter, materialComponent);
            //std::cout << "Added Rendering componenets";
        }

        if(nodes[i].parentId == -1){
            AABBComponent AABBComponent;
            RigComponent rigComponent;
            PlayerMovement playerMovementComponent;
            rigComponent.rig = model->GetRig();
            rigComponent.finalBoneMatrices.assign(rigComponent.rig.boneCounter, glm::mat4(1.0f));

            if(rigComponent.rig.GetNumberOfAnimations() > 0){
                AnimatorComponent animatorComponent;
                //std::cout << "Added Animator to root node ";
                storage.animatorStorage.emplace(entityCounter, animatorComponent);
            }

            storage.rigStorage.emplace(entityCounter, rigComponent);
            storage.AABBStorage.emplace(entityCounter, AABBComponent);
            storage.playerMovementStorage.emplace(entityCounter, playerMovementComponent);
            entityRigHolder = newEntity.GetID();
            std::cout << "Added rig to root node";
        }

        //std::cout << "Adding entity to System and Transform" << std::endl;
        storage.entityStorage.emplace(entityCounter, newEntity);
        storage.transformStorage.emplace(entityCounter, nodes[i].modelTransform);

        nodeToEntityId[i] = entityCounter;
        if(parentEntityId != -1){
            storage.entityStorage.at(parentEntityId).SetChild(entityCounter);
        }

        // std::cout << "Position: ("
        //   << storage.transformStorage[newEntity.GetID()].position.x << ", "
        //   << storage.transformStorage[newEntity.GetID()].position.y << ", "
        //   << storage.transformStorage[newEntity.GetID()].position.z << ")"
        //   << std::endl;
        //std::cout << "Creating Entity{" << newEntity.GetName() << ", " <<  newEntity.GetID()<< "}" << std::endl;

        entityCounter++;
    }
}

void StorageManager::CreateCubeEntity(){
    Entity newEntity;
    OpenGLComponent openGLComponent;
    MeshComponent meshComponent;
    MaterialComponent materialComponent;
    RigComponent rigComponent;

    meshComponent.vertices = {

        // =========================
        // FRONT (+Z)
        // =========================

        // 0
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 1
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 2
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 3
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,


        // =========================
        // BACK (-Z)
        // =========================

        // 4
        0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 5
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 6
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 7
        0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,


        // =========================
        // LEFT (-X)
        // =========================

        // 8
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 9
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 10
        -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 11
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,


        // =========================
        // RIGHT (+X)
        // =========================

        // 12
        0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 13
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 14
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 15
        0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,


        // =========================
        // TOP (+Y)
        // =========================

        // 16
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 17
        0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 18
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 19
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,


        // =========================
        // BOTTOM (-Y)
        // =========================

        // 20
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 21
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 22
        0.5f, -0.5f,  0.5f,    1.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f,

        // 23
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,    -1.0f, -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, 0.0f
    };
    meshComponent.indices = {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Back
        4, 5, 6,
        6, 7, 4,

        // Left
        8, 9, 10,
        10, 11, 8,

        // Right
        12, 13, 14,
        14, 15, 12,

        // Top
        16, 17, 18,
        18, 19, 16,

        // Bottom
        20, 21, 22,
        22, 23, 20
    };

    newEntity.SetID(entityCounter);
    storage.entityStorage.emplace(entityCounter, newEntity);
    storage.openGLStorage.emplace(entityCounter, openGLComponent);
    storage.meshStorage.emplace(entityCounter, meshComponent);
    storage.rigStorage.emplace(entityCounter, rigComponent);
    storage.materialStorage.emplace(entityCounter, materialComponent);
    entityCounter++;
}

void StorageManager::CreateCameraEntity(){
    Entity newEntity;
    TransformComponent transformComponent;
    CameraComponent camComponent;

    newEntity.SetID(entityCounter);
    storage.entityStorage.emplace(entityCounter, newEntity);
    storage.transformStorage.emplace(entityCounter, transformComponent);
    storage.cameraStorage.emplace(entityCounter, camComponent);

    std::cout << "THIS IS THE ID: Added Camera Entity {" << entityCounter << "}" << std::endl;
    entityCounter++;
}

Entity* StorageManager::GetEntityById(int id){
    return &storage.entityStorage.at(id);
}

Storage* StorageManager::GetStorage(){
    return &storage;
}


