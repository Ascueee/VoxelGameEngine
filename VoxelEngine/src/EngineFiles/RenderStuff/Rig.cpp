#include "Rig.h"

Rig::Rig(){}

void Rig::AddBone(std::string boneName, Bone bone){
    bones.emplace(boneName, bone);
}

void Rig::AddAnimation(std::string animationName, Animation animation){
    if(animations.find(animationName) == animations.end()){
        animations.emplace(animationName, animation);
    }
}

Animation* Rig::GetAnimation(std::string animationName){
    return &animations[animationName];
}

int Rig::GetNumberOfAnimations(){
    return animations.size();
}

bool Rig::BoneExist(std::string boneName)
{
    return bones.find(boneName) != bones.end();
}

Bone& Rig::GetBone(std::string boneName){
    return bones[boneName];
}

