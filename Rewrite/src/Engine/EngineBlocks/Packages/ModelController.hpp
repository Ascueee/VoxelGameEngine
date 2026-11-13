#pragma once
#include <Package.hpp>
#include <Model.hpp>
class ModelController : public Package{
    public:
        ModelController() = default;
        Model* model;
};