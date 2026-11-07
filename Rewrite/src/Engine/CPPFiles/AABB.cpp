#include <AABB.hpp>

AABB::AABB(){}

AABB::AABB(glm::vec3 position, glm::vec3 size){
    glm::vec3 halfSize = size * 0.5f;

    min = position - halfSize;
    max = position + halfSize;

    this->size = size;
}

bool AABB::IsColliding(AABB* other){
    if(min.x <= other->GetMax().x && max.x >= other->GetMin().x &&
        min.y <= other->GetMax().y && max.y >= other->GetMin().y && 
        min.z <= other->GetMax().z && max.z >= other->GetMin().z){
            return true;
    }
    else{
        return false;
    }
}

glm::vec3 AABB::GetMin() const {
    return min;
}

glm::vec3 AABB::GetMax(){
    return max;
}

glm::vec3 AABB::GetSize(){
    return size;
}

Mesh& AABB::GetMesh(){
    return debugMesh;
}


