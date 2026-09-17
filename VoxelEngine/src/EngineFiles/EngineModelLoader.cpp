#include "EngineModelLoader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

std::unordered_map<std::string, Model> EngineModelLoader::models;
EngineModelLoader::EngineModelLoader(){}

//Goes through all the files in the model folder to create all the models in the game to bake their meshes and heirarchy
void EngineModelLoader::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/VoxelEngine/src/Model";
    Assimp::Importer importer;

    importer.SetPropertyInteger(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){

        std::string modelPath = entry.path().string();
        std::string modelName = entry.path().stem();

        std::cout << "MODEL NAME: " << modelName << std::endl;
        //Need to load the model with the model path
        const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
            continue;
        }

        //Creates model and stores it in the model loader for storage
        models.emplace(modelName, Model(modelName));
        ProcessNode(scene->mRootNode, scene, -1, modelName);
        ProcessAnimation(scene, &models[modelName]);

        std::cout << "Loading: " << modelName << std::endl;
    }
}

//Goes through assimp heirarchy getting each node and creating a model node to preserce heirarchy
//Recursive so it goes thrugh the parent node and its children
void EngineModelLoader::ProcessNode(aiNode *node, const aiScene *scene, int parentID, std::string modelName){
    ModelNode modelNode;
    modelNode.parentId = parentID;
    modelNode.nodeName = node->mName.C_Str();

    std::cout << "Processing Node {" << modelNode.nodeName << "}" << std::endl;

    if(node->mNumMeshes > 0){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
        //Process assimp node into engine components for engine to use
        ProcessMesh(mesh, scene, modelNode, modelName);
    }

    ProcessTransform(node, modelNode);
    int currentNodeID = static_cast<int>(models[modelName].GetNodes().size());
    models[modelName].GetNodes().push_back(modelNode);

    //Checks if the node has any more meshes to add
    for (unsigned int i = 1; i < node->mNumMeshes; i++) {
        ModelNode extra;
        extra.parentId = currentNodeID;
        extra.modelTransform = TransformComponent();
        ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, extra, modelName);

        int extraId = static_cast<int>(models[modelName].GetNodes().size());

        models[modelName].GetNodes().push_back(extra);
        models[modelName].GetNodes()[currentNodeID].children.push_back(extraId);
    }

    //Goes through the children of the node to create ModelNodes
    for (int i = 0; i < node->mNumChildren; i++) {
        int childId = static_cast<int>(models[modelName].GetNodes().size());
        ProcessNode(node->mChildren[i], scene, currentNodeID, modelName);
        models[modelName].GetNodes()[currentNodeID].children.push_back(childId);
    }
}

void EngineModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene, ModelNode& modelNode, std::string modelName){
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    MeshComponent modelMesh;

    for(int i = 0; i < mesh->mNumVertices; i++){
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        if(mesh->mTextureCoords[0]){
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);

        }
        else{
            vertices.push_back(0);
            vertices.push_back(0);
        }
        //Need to create bone data as default
        vertices.push_back(-1);
        vertices.push_back(-1);
        vertices.push_back(-1);
        vertices.push_back(-1);

        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
    }

    for(int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    //Bone extraction needs to happen here
    ExtractRig(mesh, scene, vertices, modelName);

    modelMesh.mesh.vertices = vertices;
    modelMesh.mesh.indices = indices;

    modelNode.modelMesh = modelMesh;
}

void EngineModelLoader::ProcessTransform(aiNode* node, ModelNode& modelNode){
    TransformComponent transform;

    glm::mat4 importedModel = glm::mat4(
        node->mTransformation.a1, node->mTransformation.b1, node->mTransformation.c1, node->mTransformation.d1,
        node->mTransformation.a2, node->mTransformation.b2, node->mTransformation.c2, node->mTransformation.d2,
        node->mTransformation.a3, node->mTransformation.b3, node->mTransformation.c3, node->mTransformation.d3,
        node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4, node->mTransformation.d4
    );

    glm::quat orientation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(importedModel, transform.scale, orientation, transform.position, skew, perspective);
    transform.rotation = glm::eulerAngles(orientation);

    modelNode.modelTransform = transform;
}

void EngineModelLoader::ExtractRig(aiMesh* mesh, const aiScene* scene, std::vector<float>& vertices, std::string modelName){
    Rig& modelRig = models[modelName].GetRig();

    for(int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++){

        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
    
        if(!modelRig.BoneExist(boneName)){
            Bone newBone;
            newBone.boneID = modelRig.boneCounter;
            aiMatrix4x4 aiMat = mesh->mBones[boneIndex]->mOffsetMatrix;
            newBone.offSet = glm::mat4(
                aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
                aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
                aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
                aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
            );
            std::cout << "Added Bone {" << boneName << "}" << std::endl;
            modelRig.AddBone(boneName, newBone);
            boneID = newBone.boneID;
            modelRig.boneCounter++;
        }
        else{
            boneID = modelRig.GetBone(boneName).boneID;
        }

        //Add weights and ids to vertex data
        auto weights = mesh->mBones[boneIndex]->mWeights;
        unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;
        for (unsigned int weightIndex = 0; weightIndex < numWeights; weightIndex++) {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;

            int base = vertexId * 13;// 3 position + 2 uvs + 4 boneIDs + 4 weights per vertex
            int idOffset = base + 5;
            int weightOffset = idOffset + 4;

            //Adds the updates weights to the vertex data
            for (int slot = 0; slot < 4; slot++) {
                if (vertices[idOffset + slot] < 0.0f)
                { 
                    vertices[idOffset + slot] = static_cast<float>(boneID);
                    vertices[weightOffset + slot] = weight;
                    break;
                }
            }
        }
    }
}

void EngineModelLoader::ProcessAnimation(const aiScene *scene, Model* model){
    //For each animation I need to process the keyframes and add it to the animation data st
    for(int i = 0; i < scene->mNumAnimations; i++){
        aiAnimation* animation = scene->mAnimations[i];
        Animation newAnimation;
        std::cout << "Loading Animation {" << animation->mName.C_Str() << "}, ";
        std::cout << "Animation Duration {" << animation->mDuration << "}, ";
        std::cout << "Animation TicksPerSecond {" << animation->mTicksPerSecond << "}, ";
        std::cout << "Animation Channels {" << animation->mNumChannels << "}" << std::endl;
        std::cout << " Number of Bones in Rig  { " << model->GetRig().boneCounter << "}" << std::endl;
        newAnimation.name = animation->mName.C_Str();
        newAnimation.duration = animation->mDuration;
        newAnimation.ticksPerSecond = animation->mTicksPerSecond;

        //Now need to go into each channel of the animation and extract animation data
        for(int j = 0; j < animation->mNumChannels; j++){
            KeyFrameData keyFrame;
            aiNodeAnim* channel = animation->mChannels[j];
            std::cout << "Current Channel {" << channel->mNodeName.C_Str() << "}, ";
            std::cout << "Position Keys {" << channel->mNumPositionKeys << "}, ";
            std::cout << "RotationKeys {" << channel->mNumRotationKeys << "}, ";
            std::cout << "ScaleKeys {" << channel->mNumScalingKeys << "}" << std::endl;

            //Extracts keyframe data
            for(int posKeyIndex = 0; posKeyIndex < channel->mNumPositionKeys; posKeyIndex++){
                VectorKey positionKey;
                aiVectorKey keyFramePos = channel->mPositionKeys[posKeyIndex];
                positionKey.time = keyFramePos.mTime;
                positionKey.keyValue = glm::vec3(keyFramePos.mValue.x, keyFramePos.mValue.y, keyFramePos.mValue.z);

                keyFrame.positionKeys.push_back(positionKey);
            }

            for(int rotKeyIndex = 0; rotKeyIndex < channel->mNumRotationKeys; rotKeyIndex++){
                RotationKey rotationKey;
                aiQuatKey keyFrameRot = channel->mRotationKeys[rotKeyIndex];

                rotationKey.time = keyFrameRot.mTime;
                rotationKey.keyValue = glm::quat(keyFrameRot.mValue.w, keyFrameRot.mValue.x, keyFrameRot.mValue.y, keyFrameRot.mValue.z);
                keyFrame.rotationKeys.push_back(rotationKey);
            }

            for(int scaleKeyIndex = 0; scaleKeyIndex < channel->mNumScalingKeys; scaleKeyIndex++){
                VectorKey scaleKey;
                aiVectorKey keyFrameScale = channel->mScalingKeys[scaleKeyIndex];
                scaleKey.time = keyFrameScale.mTime;
                scaleKey.keyValue = glm::vec3(keyFrameScale.mValue.x, keyFrameScale.mValue.y, keyFrameScale.mValue.z);

                keyFrame.scaleKeys.push_back(scaleKey);
            }

            newAnimation.keyFrameData[channel->mNodeName.C_Str()] = keyFrame;
        }

        std::cout << "Added Animation {" << newAnimation.name << "} To {" << model->GetModelName() << "}" << std::endl;
        //Adds animation to the rig for the animator to use
        model->GetRig().AddAnimation(newAnimation.name, newAnimation);
    };
}

Model* EngineModelLoader::GetModel(std::string modelName){
    auto it = models.find(modelName);
    if (it == models.end()) {
        std::cout << "ERROR: No model loaded with name: " << modelName << std::endl;
        return nullptr;
    }
    return &it->second;
}