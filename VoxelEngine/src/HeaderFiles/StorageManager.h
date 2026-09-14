#pragma once
#include <iostream>
#include "Storage.h" 
#include "Model.h"

class StorageManager{
    public:
        StorageManager();
        static void CreateBlankEntity();
        static void CreateCubeEntity();
        static void CreateDebugAABBEntity(Entity* debugEnt, glm::vec3 offSet = glm::vec3(0,0,0));
        static void BuildModel(Model* model);
        
        static Entity* GetEntityById (int entityID);
        static Storage* GetStorage();

    private:
        static int entityCounter;
        static Storage storage;
};