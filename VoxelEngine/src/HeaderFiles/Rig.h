#pragma once
#include <unordered_map>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Animation.h"

struct Bone{
    int boneID;
    glm::mat4 offSet;
    glm::mat4 finalBoneMatrix;
};

class Rig{
    public:
        Rig();
        void AddBone(std::string boneName, Bone bone);
        bool BoneExist(std::string boneName);
        void AddAnimation(std::string animationName, Animation animation);
        Animation* GetAnimation(std::string animationName);
        int GetNumberOfAnimations();
        Bone& GetBone(std::string boneName);
        int boneCounter = 0;
    private:
        std::unordered_map<std::string, Bone> bones;
        std::unordered_map<std::string, Animation> animations;
};