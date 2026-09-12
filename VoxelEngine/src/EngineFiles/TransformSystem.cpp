#include "TransformSystem.h"

TransformSystem::TransformSystem(Storage* storage) : System(storage){}

void TransformSystem::Run(){
    for (auto& [id, entity] : storage->entityStorage) {        
        if (entity.GetParentID() == -1)
        {
            UpdateTransform(entity);
        }
    }
}

//Used to update the entities model matrix
void TransformSystem::UpdateTransform(Entity ent){
    TransformComponent& entityTransform = storage->transformStorage[ent.GetID()];
    entityTransform.model = glm::mat4(1.0f);

    entityTransform.model = glm::translate(entityTransform.model, entityTransform.position);
    entityTransform.model = glm::rotate(entityTransform.model, entityTransform.rotation.x, glm::vec3(1,0,0));
    entityTransform.model = glm::rotate(entityTransform.model, entityTransform.rotation.y, glm::vec3(0,1,0));
    entityTransform.model = glm::rotate(entityTransform.model, entityTransform.rotation.z, glm::vec3(0,0,1));
    entityTransform.model = glm::scale(entityTransform.model, entityTransform.scale);

    if(ent.GetParentID() != -1){
        TransformComponent& parentTransform = storage->transformStorage[ent.GetParentID()];
        entityTransform.model = parentTransform.model * entityTransform.model;
    }

    for(int i = 0; i < ent.GetChildren().size(); i++){
        UpdateTransform(storage->entityStorage[ent.GetChildren()[i]]);
    }
}