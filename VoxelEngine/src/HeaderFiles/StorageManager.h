#pragma once
#include <iostream>
#include "Storage.h" 
#include "Model.h"

class StorageManager{
    public:
        StorageManager();
        static Entity* CreateBlankEntity();
        static Entity* CreateCubeEntity();
        static Entity* CreateCameraEntity();
        static Entity* CreateChunkEntity();
        static Entity* BuildModel(Model* model);
        
        static Entity* GetEntityById (int entityID);
        static Storage* GetStorage();

    private:
        static int entityCounter;
        static Storage storage;
};