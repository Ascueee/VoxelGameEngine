#include <BlockModelStorage.hpp>

std::unordered_map<std::string, BlockMesh> BlockModelStorage::models;
BlockModelStorage::BlockModelStorage(){}

//Load all the obj/fbx files in the block model folders and build their blockModelMesh
void BlockModelStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/BlockModels";
    Assimp::Importer modelImporter;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){

        if(!entry.is_regular_file()){
            continue;
        }    
        std::string extension = entry.path().extension().string();

        if (extension != ".obj" && extension != ".fbx"){
            std::cout << "[BlockModel Storage] skipping file not obj or fbx\n";
            continue;
        }

        const aiScene* scene = modelImporter.ReadFile(entry.path().string(),
             aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            std::cout << "[BlockModelStorage] {" << modelImporter.GetErrorString() << "} \n";
            return;
        }

        std::cout << "[BlockModelStorage] Processing Model {" << entry.path().filename().string() << "} \n";

        std::string modelName = entry.path().filename().string();

        //Add A block mesh that represents the model
        models.emplace(modelName, BlockMesh());

        //Now need to process the node and load it into the block model mesh
        ProcessNode(scene->mRootNode, scene, modelName);
    }
}

void BlockModelStorage::ProcessNode(aiNode *node, const aiScene *scene, std::string modelName){
    BlockMesh& blockMesh = models[modelName];

    for(int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        std::cout << "[BlockModel Storage] Processing mesh {" << mesh->mName.C_Str() << "} \n";
        ProcessBlockMesh(blockMesh, mesh, scene);
    }

    for(int i = 0; i < node->mNumChildren; i++){
        ProcessNode(node->mChildren[i], scene, modelName);
    }
}

void BlockModelStorage::ProcessBlockMesh(BlockMesh& blockMesh, aiMesh *mesh, const aiScene *scene){
    //This will add to the blockMesh vertices
    std::vector<float>& vertices = blockMesh.GetVertices();

    unsigned int baseVertex = vertices.size() / 5;
    //Extract vertex data
    for(int i = 0; i < mesh->mNumVertices; i++){

        //Adds the position to the vertices
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        //Need to add UVS to the vertices data
        if(mesh->mTextureCoords[0]){
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else{
            vertices.push_back(0);
            vertices.push_back(0);
        }
    }

    std::vector<unsigned int> faceIndices;
    //Extract Indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        std::cout << "Process Mesh Indices for Face {" << mesh->mName.C_Str() << "}, Indices are{ ";
        //A blockMesh is broken up into faces
        //Need to fill each faceIndices entry with faceIndice data and then add it to the faceIndices dictionary
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            std::cout << face.mIndices[j] << ", ";
            faceIndices.push_back(face.mIndices[j] + baseVertex);
        }

        std::cout << "} \n";
        
    }
    //Add the face to the faces dictionary
    blockMesh.AddFace(mesh->mName.C_Str(), faceIndices);
}

BlockMesh* BlockModelStorage::GetBlockModel(std::string modelName){
    auto it = models.find(modelName);

    if (it != models.end())
    {
        return &it->second;
    }

    return nullptr;
}

