#include <Entity.hpp>

template <typename T>
void Entity::AddPackage() {
    
    std::type_index key(typeid(T));

    if (packages.find(key) != packages.end()){
        return;
    }

    packages[key] = new T();
}

template <typename T>
T* Entity::GetPackage() {

    auto it = packages.find(std::type_index(typeid(T)));

    if (it == packages.end()){
        return nullptr;
    } 
    else{
        return static_cast<T*>(it->second);
    }
}