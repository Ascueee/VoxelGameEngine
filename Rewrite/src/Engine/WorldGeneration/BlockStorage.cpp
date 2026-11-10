#include <BlockStorage.hpp>

std::unordered_map<ushort, Block> BlockStorage::blocks;
BlockStorage::BlockStorage() {}

void BlockStorage::Load(){
    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Blocks";

   for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::ifstream file(entry.path());

        if(!file.is_open()){
            std::cout << "[Block Storage] Failed to open file \n";
            continue;
        }

        nlohmann::json data;
        file >> data;
        Block block = Block(data["name"].get<std::string>(), data["id"].get<int>(), 
        data["textureIds"].get<std::vector<unsigned int>>(), data["blockType"].get<BlockType>(), data["cullable"].get<bool>());

        std::cout << "[Block Storage] Loaded {" << data["name"].get<std::string>() << "} \n";
        blocks.emplace(data["id"].get<int>(), block);
   }
}

Block* BlockStorage::GetBlock(ushort id){
    if(blocks.count(id) > 0){
        return &blocks.at(id);
    }
    else{
        return &blocks.at(0); 
    }
}