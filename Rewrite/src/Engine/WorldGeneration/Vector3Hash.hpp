#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <cstddef>

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        std::size_t h3 = std::hash<float>{}(v.z);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};