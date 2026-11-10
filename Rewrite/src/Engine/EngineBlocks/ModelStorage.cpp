#include <ModelStorage.hpp>

std::unordered_map<std::string, Model> ModelStorage::models;
ModelStorage::ModelStorage(){}

void ModelStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Models";
    Assimp::Importer modelImporter;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){

        if(!entry.is_regular_file()){
            continue;
        }    
        std::string extension = entry.path().extension().string();

        if (extension != ".obj" && extension != ".fbx"){
            std::cout << "[Model Storage] skipping file not obj or fbx\n";
            continue;
        }

        const aiScene* scene = modelImporter.ReadFile(entry.path().string(),
             aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            std::cout << "[Model Storage] {" << modelImporter.GetErrorString() << "} \n";
            return;
        }

        std::string modelName = entry.path().stem().string();
        std::cout << "[Model Storage] Loading Model {" << modelName << "} \n";
        models.emplace(modelName, Model());

        ProcessNode(scene->mRootNode, scene, modelName);
    }
}


void ModelStorage::ProcessNode(aiNode *node, const aiScene *scene, std::string modelName){
    Model& model = models[modelName];
    for(int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[i];
        std::cout << "Processing Model Mesh {" << mesh->mName.C_Str() << "} \n";
        model.AddMesh(mesh->mName.C_Str(), ProcessMesh(mesh, scene, &model, mesh->mName.C_Str()));
    }

    for(int i = 0; i < node->mNumChildren; i++){
        ProcessNode(node->mChildren[i], scene, modelName);
    }
}

Mesh ModelStorage::ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* model, std::string meshName){
    Mesh& modelMesh = *model->GetMesh(meshName);

    for(int i = 0; i < mesh->mNumVertices; i++){
        modelMesh.GetVertices().push_back(mesh->mVertices[i].x);
        modelMesh.GetVertices().push_back(mesh->mVertices[i].y);
        modelMesh.GetVertices().push_back(mesh->mVertices[i].z);

        if(mesh->HasTextureCoords(0)){
            modelMesh.GetVertices().push_back(mesh->mTextureCoords[0][i].x);
            modelMesh.GetVertices().push_back(mesh->mTextureCoords[0][i].y);
        }
        else{
            modelMesh.GetVertices().push_back(0.0f);
            modelMesh.GetVertices().push_back(0.0f);
        }

        if(mesh->HasNormals()){
            modelMesh.GetVertices().push_back(mesh->mNormals[i].x);
            modelMesh.GetVertices().push_back(mesh->mNormals[i].y);
            modelMesh.GetVertices().push_back(mesh->mNormals[i].z);
        }
    }

    for(int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(int j = 0; j < face.mNumIndices; j++){
            modelMesh.GetIndices().push_back(face.mIndices[j]);        
        }
    }
    return modelMesh;
}

Model* ModelStorage::GetModel(std::string name){
    if(models.find(name) != models.end()){
        return &models.at(name);
    }
    else{
        return &models.at("Cube");
    }
}
