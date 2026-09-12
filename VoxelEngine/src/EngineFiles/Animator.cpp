#include "Animator.h"

Storage* Animator::storage;

Animator::Animator(){}
void Animator::RunAnimation(Entity* ent, std::string animationName, float deltaTime){
    RigComponent& rigComponent = storage->rigStorage[ent->GetID()];
    AnimatorComponent& animatorComponent = storage->animatorStorage[ent->GetID()];
    //calls a refrence to the rig because it needs to change the bone data(final matrix)
    Rig& rig = rigComponent.rig;
    Animation* currentAnimation = rig.GetAnimation(animationName);

    float ticksPerSecond;

    if (currentAnimation->ticksPerSecond != 0.0f)
        ticksPerSecond = currentAnimation->ticksPerSecond;
    else
        ticksPerSecond = 25.0f;
    
    animatorComponent.currentTime += deltaTime * ticksPerSecond;

    animatorComponent.currentTime = fmod(animatorComponent.currentTime, currentAnimation->duration);

    //std::cout << "Evalutating Animation {" << currentAnimation->name << "}" << std::endl;
    EvaluateAnimation(ent, rig, rigComponent, currentAnimation, animatorComponent.currentTime);
}

void Animator::EvaluateAnimation(Entity* ent, Rig& rig, RigComponent& rigComponent, Animation* animation, float animationTime){
    int entID = ent->GetID();
    int parentID = ent->GetParentID();

    glm::mat4 boneLocalMatrix = glm::mat4(1.0f);

    //Find if the entity has a name that matches one in the rig
    if(rig.BoneExist(ent->GetName())){
        //std::cout << "Animating {" << ent->GetName() << "} " << std::endl;
        //Now that an animation has been found with a bone I need to evalute it
        KeyFrameData keyFrame = animation->keyFrameData[ent->GetName()];

        //Build the bones localMatrix here
        glm::vec3 position = InterpolatePosition(keyFrame, animationTime);
        glm::quat rotation = InterpolateRotation(keyFrame, animationTime);
        glm::vec3 scale = InterpolateScale(keyFrame, animationTime);

        boneLocalMatrix = glm::translate(boneLocalMatrix, position);
        boneLocalMatrix *= glm::mat4_cast(rotation);
        boneLocalMatrix = glm::scale(boneLocalMatrix, scale);
    }
    else{
        //If node doesn't have animation keyframes, fallback to static transform component
        TransformComponent& t = storage->transformStorage[entID];
        boneLocalMatrix = glm::translate(boneLocalMatrix, t.position);
        boneLocalMatrix = glm::rotate(boneLocalMatrix, t.rotation.x, glm::vec3(1,0,0));
        boneLocalMatrix = glm::rotate(boneLocalMatrix, t.rotation.y, glm::vec3(0,1,0));
        boneLocalMatrix = glm::rotate(boneLocalMatrix, t.rotation.z, glm::vec3(0,0,1));
        boneLocalMatrix = glm::scale(boneLocalMatrix, t.scale);
    }

    //Now its time to get the bones global Matrix
    glm::mat4 parentGlobalMatrix = glm::mat4(1.0f);
    if(parentID != -1){
        parentGlobalMatrix = storage->transformStorage[parentID].model;
    }

    glm::mat4 boneGlobalMatrix = parentGlobalMatrix * boneLocalMatrix;

    //Store animated global transform so child entities pull the updated parent matrix
    storage->transformStorage[entID].model = boneGlobalMatrix;

    if(rig.BoneExist(ent->GetName())){
        Bone& currentBone = rig.GetBone(ent->GetName());
        currentBone.finalBoneMatrix = boneGlobalMatrix * currentBone.offSet;

        if(currentBone.boneID >= 0 && currentBone.boneID < (int)rigComponent.finalBoneMatrices.size()){
            rigComponent.finalBoneMatrices[currentBone.boneID] = currentBone.finalBoneMatrix;
        }
    }

    //need to recursivly go through the entities children
    for(int i = 0; i < ent->GetChildren().size(); i++){
        EvaluateAnimation(&storage->entityStorage[ent->GetChildren()[i]], rig, rigComponent, animation, animationTime);
    }
}

template<typename T>
int Animator::GetKeyIndex(const std::vector<T>& keys, float animationTime){
    for(int i = 0; i < (int)keys.size() - 1; i++){
        if(animationTime < keys[i + 1].time)
             return i;
    }
    return (int)keys.size() - 2;
}

float Animator::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime){
    float framesDiff = nextTimeStamp - lastTimeStamp;
    if(framesDiff <= 0.0f) return 0.0f; 
    float factor = (animationTime - lastTimeStamp) / framesDiff;
    return std::clamp(factor, 0.0f, 1.0f);
}

glm::vec3 Animator::InterpolatePosition(const KeyFrameData& keyFrame, float animationTime){
    const std::vector<VectorKey>& keys = keyFrame.positionKeys;
    if(keys.size() <= 1) return keys.empty() ? glm::vec3(0.0f) : keys[0].keyValue;

    int p0 = GetKeyIndex(keys, animationTime);
    int p1 = p0 + 1;
    float factor = GetScaleFactor(keys[p0].time, keys[p1].time, animationTime);
    return glm::mix(keys[p0].keyValue, keys[p1].keyValue, factor);
}

glm::quat Animator::InterpolateRotation(const KeyFrameData& keyFrame, float animationTime){
    const std::vector<RotationKey>& keys = keyFrame.rotationKeys;
    if(keys.size() <= 1) return keys.empty() ? glm::quat(1,0,0,0) : glm::normalize(keys[0].keyValue);

    int p0 = GetKeyIndex(keys, animationTime);
    int p1 = p0 + 1;
    float factor = GetScaleFactor(keys[p0].time, keys[p1].time, animationTime);
    return glm::normalize(glm::slerp(keys[p0].keyValue, keys[p1].keyValue, factor));
}

glm::vec3 Animator::InterpolateScale(const KeyFrameData& keyFrame, float animationTime){
    const std::vector<VectorKey>& keys = keyFrame.scaleKeys;
    if(keys.size() <= 1) return keys.empty() ? glm::vec3(1.0f) : keys[0].keyValue;

    int p0 = GetKeyIndex(keys, animationTime);
    int p1 = p0 + 1;
    float factor = GetScaleFactor(keys[p0].time, keys[p1].time, animationTime);
    return glm::mix(keys[p0].keyValue, keys[p1].keyValue, factor);
}
