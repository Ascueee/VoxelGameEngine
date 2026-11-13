#pragma once
#include <iostream>
#include <sol/sol.hpp>
#include <Entity.hpp>

class Script{
    public:
        Script();
        void SetScriptLoad(sol::protected_function scriptLoad);
        void SetScriptUpdate(sol::protected_function scriptUpdate);
        void Load(Entity* entity);
        void Update(Entity* entity);
    private:
        sol::protected_function scriptLoad;
        sol::protected_function scriptUpdate;
};
