#include <Script.hpp>

Script::Script(){}

void Script::SetScriptLoad(sol::protected_function scriptLoad){
    this->scriptLoad = scriptLoad;
}

void Script::SetScriptUpdate(sol::protected_function scriptUpdate){
    this->scriptUpdate = scriptUpdate;
}

void Script::Load(Entity* entity){
    sol::protected_function_result result = scriptLoad(entity);

    if (!result.valid()) {
        sol::error err = result;
        std::cout << "[Lua Error] " << err.what() << std::endl;
    }

}

void Script::Update(Entity* entity){
    sol::protected_function_result result = scriptUpdate(entity);

    if (!result.valid()) {
        sol::error err = result;
        std::cout << "[Lua Error] " << err.what() << std::endl;
    }

}
