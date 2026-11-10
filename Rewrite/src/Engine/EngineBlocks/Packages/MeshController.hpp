#pragma once
#include <Package.hpp>
#include <Mesh.hpp>
class MeshController : public Package{
    public:
        MeshController() = default;
        Mesh* mesh;
};