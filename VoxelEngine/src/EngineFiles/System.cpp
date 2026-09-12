#include <iostream>
#include "System.h"

System::System(Storage* storage){
    this->storage = storage;
}

void System::Load(){
    std::cout << "Load" << std::endl;
}

void System::Run() {
    for (auto& [id, entity] : storage->entityStorage) {

        TestComponent& test = storage->testStorage[id];

        std::cout << "The Entity {" << storage->entityStorage[id].GetID() << "} has a message: " << test.msg << std::endl;
    }
}