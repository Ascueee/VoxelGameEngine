#include "StorageManager.h"

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
            ColorComponent colorComponent;
            nodes[i].modelMesh.rigRef = entityRigHolder;
            storage.openGLStorage.emplace(entityCounter, openGLComponent);
            storage.colorStorage.emplace(entityCounter, colorComponent);
            storage.meshStorage.emplace(entityCounter, nodes[i].modelMesh);
            //std::cout << "Added Rendering componenets";
        }

        if(nodes[i].parentId == -1){
            AABBComponent AABBComponent;
            RigComponent rigComponent;
            rigComponent.rig = model->GetRig();
            rigComponent.finalBoneMatrices.assign(rigComponent.rig.boneCounter, glm::mat4(1.0f));

            if(rigComponent.rig.GetNumberOfAnimations() > 0){
                AnimatorComponent animatorComponent;
                //std::cout << "Added Animator to root node ";
                storage.animatorStorage.emplace(entityCounter, animatorComponent);
            }

            storage.rigStorage.emplace(entityCounter, rigComponent);
            storage.AABBStorage.emplace(entityCounter, AABBComponent);
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
    ColorComponent colorComponent;
    RigComponent rigComponent;

    meshComponent.vertices = {
        // 0
        -0.5f, -0.5f, -0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 1
        0.5f, -0.5f, -0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 2
        0.5f,  0.5f, -0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 3
        -0.5f,  0.5f, -0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 4
        -0.5f, -0.5f,  0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 5
        0.5f, -0.5f,  0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 6
        0.5f,  0.5f,  0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f,
        // 7
        -0.5f,  0.5f,  0.5f,   -1.0f, -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 0.0f, 0.0f
    };
    meshComponent.indices = {
        // Front
        4, 5, 6,
        6, 7, 4,

        // Back
        0, 1, 2,
        2, 3, 0,

        // Left
        0, 4, 7,
        7, 3, 0,

        // Right
        1, 5, 6,
        6, 2, 1,

        // Top
        3, 7, 6,
        6, 2, 3,

        // Bottom
        0, 4, 5,
        5, 1, 0
    };

    newEntity.SetID(entityCounter);
    storage.entityStorage.emplace(entityCounter, newEntity);
    storage.openGLStorage.emplace(entityCounter, openGLComponent);
    storage.meshStorage.emplace(entityCounter, meshComponent);
    storage.colorStorage.emplace(entityCounter, colorComponent);
    storage.rigStorage.emplace(entityCounter, rigComponent);
    entityCounter++;
}

Entity* StorageManager::GetEntityById(int id){
    return &storage.entityStorage.at(id);
}

Storage* StorageManager::GetStorage(){
    return &storage;
}


