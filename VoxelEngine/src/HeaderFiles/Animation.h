#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <unordered_map>

struct VectorKey{
    float time;
    glm::vec3 keyValue;
};

struct RotationKey{
    float time;
    glm::quat keyValue;
};

struct KeyFrameData{
    std::vector<VectorKey> positionKeys;
    std::vector<RotationKey> rotationKeys;
    std::vector<VectorKey> scaleKeys;
};

struct Animation{
    std::string name;
    float duration;
    float ticksPerSecond;
    //Stores a keyframe for each bone in the rig
    std::unordered_map<std::string, KeyFrameData> keyFrameData;

};
