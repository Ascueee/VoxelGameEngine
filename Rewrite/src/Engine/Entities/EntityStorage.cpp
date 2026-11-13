#include <EntityStorage.hpp>

std::unordered_map<std::string, Entity> EntityStorage::entities;
EntityStorage::EntityStorage(){}

void EntityStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Entities";
    
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::ifstream file(entry.path());
        if(!file.is_open()){
            std::cout << "[EntityManager] Failed to open file \n";
            continue;
        }

        nlohmann::json data;
        file >> data;

        //Add componenents  here
        Entity& entity = entities[data["name"]];
        entity.SetName(data["name"]);
        for (auto& package : data["packages"]){
            std::cout << "[Entity Storage] loading componenet" << package << std::endl;
            if(package == "Transform"){
                entity.AddPackage<Transform>();
            }
            else if(package == "ModelController"){
                entity.AddPackage<ModelController>();
            }
            else if(package == "CameraController"){
                entity.AddPackage<CameraController>();
            }
            else if(package == "ScriptController"){
                entity.AddPackage<ScriptController>();
                entity.GetPackage<ScriptController>()->script = ScriptStorage::GetScript(data["script"]);
            }
        }
    }
}

//This is where the entities will get ready for the engine
void EntityStorage::LoadEntities(){
    for(auto& [name, entity] : entities){
        if(entity.GetPackage<ScriptController>() != nullptr){
            entity.GetPackage<ScriptController>()->script->Load(&entity);
        }

        if(entity.GetPackage<ModelController>() != nullptr){

            for(auto& [name, node] : *entity.GetPackage<ModelController>()->model->GetNodes()){
                Renderer::LoadMesh(node.mesh);
            }
        }
    }
}

void EntityStorage::UpdateEntities(){
    for(auto& [name, entity] : entities){
        if(entity.GetPackage<ScriptController>() != nullptr){
            entity.GetPackage<ScriptController>()->script->Update(&entity);
        }
    }
}

void EntityStorage::GeometryPass(){
    for(auto& [name, entity] : entities){
        if(entity.GetPackage<ModelController>() != nullptr){
            for(auto& [name, node] : *entity.GetPackage<ModelController>()->model->GetNodes()){
                Renderer::GeometryPassEntity(node.mesh, entity.GetPackage<Transform>());
            }
        }
    }
}

void EntityStorage::ForwardPass(){
    for(auto& [name, entity] : entities){
        if(entity.GetPackage<ModelController>() != nullptr){
            for(auto& [name, node] : *entity.GetPackage<ModelController>()->model->GetNodes()){
                Renderer::ForwardPassEntity(node.mesh, entity.GetPackage<Transform>());
            }
        }
    }
}

//Creates empty entity for the engine to use
void EntityStorage::CreateEntity(std::string name){
    entities.emplace(name, Entity());

    entities.at(name).SetName(name);
}

Entity* EntityStorage::GetEntity(std::string name){
    if(entities.find(name) != entities.end()){
        return &entities.at(name);
    }
    else{
        std::cout << "[Entity Manager] Could not find entity \n";
        return nullptr;
    }
}