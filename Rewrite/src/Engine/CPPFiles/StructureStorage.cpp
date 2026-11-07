#include <StructureStorage.hpp>

std::unordered_map<std::string, TreeStructure> StructureStorage::structures;
StructureStorage::StructureStorage(){}

void StructureStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Structures";


   for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::ifstream file(entry.path());

        if(!file.is_open()){
            std::cout << "[Structure Storage] Failed to open file \n";
            continue;
        }

        nlohmann::json data;
        file >> data;

        TreeStructure structure = TreeStructure();
        structure.SetName(data["name"].get<std::string>());
        structure.SetWidth(data["Settings"]["width"].get<int>());
        structure.SetHeight(data["Settings"]["height"].get<int>());
        structure.SetWoodId(data["Settings"]["woodId"].get<int>());
        structure.SetBlocks(data["Canopy"]["blocks"].get<std::unordered_map<std::string, ushort>>());

        structures.emplace(structure.GetName(), structure);
        std::cout << "[Structure Storage] Loaded: " << data["name"].get<std::string>() << "\n";
   }
}


TreeStructure* StructureStorage::GetStructure(std::string name){
    auto it = structures.find(name);
    if (it != structures.end())
    {
        return &it->second;
    }
    return nullptr;
}