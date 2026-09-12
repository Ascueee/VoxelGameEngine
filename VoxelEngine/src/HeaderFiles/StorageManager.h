#pragma once
#include <iostream>
#include "Storage.h" 
#include "Model.h"

class StorageManager{
    public:
        StorageManager();
        void CreateBlankEntity();
        void CreateCubeEntity();
        void BuildModel(Model* model);
        
        Entity* GetEntityById (int entityID);
        Storage* GetStorage();

    private:
        int entityCounter;
        Storage storage;
};