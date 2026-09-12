#pragma once
#include "Storage.h"
class System{
    public:
        System();
        System(Storage* storage);
        virtual void Load();
        virtual void Run();
    protected:
        Storage* storage;
};