#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "System.h"

class TransformSystem : public System{
    public:
        TransformSystem(Storage* storage);
        void Run() override;
        void UpdateTransform(Entity ent);
    private:
};