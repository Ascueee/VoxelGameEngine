#include <AABB.hpp>

AABB::AABB(){}

AABB::AABB(glm::vec3 position, glm::vec3 size){
    glm::vec3 halfSize = size * 0.5f;

    min = position - halfSize;
    max = position + halfSize;

    this->size = size;
}

glm::vec3 AABB::GetMin(){
    return min;
}

glm::vec3 AABB::GetMax(){
    return max;
}

glm::vec3 AABB::GetSize(){
    return size;
}


