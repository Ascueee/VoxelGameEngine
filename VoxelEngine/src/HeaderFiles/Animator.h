#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Storage.h"
//ANIMATION PSEUDO CODE/PLAN:
//When animating the globals
//Use entity transforms
//For the Animation
//I will go down the entity heirarchy
//IF an entitys name matches a bone in the rigs unorderd map bones(the keys are the bone names)
//Then its time to animate
//To animate 1) get the bones local transform from lerping keyframes
//I can use the entities transform matrix instead of the parents because the transforms model is already affected by the parent
//BUT I NEED TO RUN THE ANIMATION AFTER THE TRANSFORM EVENTS TO BE ABLE TO USE transform.model
//To animate 2) then need to calcualte the global matrix which is = transform.modelMatrix * bone.localMatrix
//to animate 3) Then need to get the finalBoneMatrix = global * bone.localMatrix;
//to animate 4) Then need to updates the rigs finalBoneMatrices list.

class Animator{
    public: 
        Animator();
        static void RunAnimation(Entity* ent, std::string animationName, float deltaTime);
        //This will recusivly go through the entity heirarchy to animate the specific bones relative to the entity transform system
        static void EvaluateAnimation(Entity* ent, AnimatorComponent& animator, RigComponent& rigComponent, int rootEntity, Animation* animation, float animationTime);
        static Storage* storage;
    private:

        static float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
        static glm::vec3 InterpolatePosition(const KeyFrameData& keyFrame, float animationTime);
        static glm::quat InterpolateRotation(const KeyFrameData& keyFrame, float animationTime);
        static glm::vec3 InterpolateScale(const KeyFrameData& keyFrame, float animationTime);
        template<typename T>
        static int GetKeyIndex(const std::vector<T>& keys, float animationTime);  

};