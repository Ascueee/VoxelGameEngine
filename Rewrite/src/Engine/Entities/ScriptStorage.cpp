#include <ScriptStorage.hpp>

sol::state ScriptStorage::luaReader;
std::unordered_map<std::string, sol::protected_function> ScriptStorage::scripts;
ScriptStorage::ScriptStorage(){}

void ScriptStorage::Load(){
    luaReader.open_libraries(sol::lib::base);

    //Data types
    luaReader.new_usertype<Entity>(
        "Entity",
        sol::constructors<Entity()>(),
        "GetName",[](Entity& e){return e.GetName();},
        "transform", sol::property([](Entity& e){ return e.GetPackage<Transform>();}),
        "camera", sol::property([](Entity& e){ return e.GetPackage<CameraController>();})
    );

    luaReader.new_usertype<glm::vec3>(
        "Vector3",
        sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", sol::property([](glm::vec3& v){ return v.x; }, [](glm::vec3& v, float f){ v.x = f; }),
        "y", sol::property([](glm::vec3& v){ return v.y; }, [](glm::vec3& v, float f){ v.y = f; }),
        "z", sol::property([](glm::vec3& v){ return v.z; }, [](glm::vec3& v, float f){ v.z = f; }),
        sol::meta_function::addition,    [](const glm::vec3& a, const glm::vec3& b){ return a + b; }
    );

    luaReader.new_usertype<Transform>(
        "Transform",
        sol::constructors<Transform()>(),
        "position", sol::property([](Transform* t){ return t->position; }, [](Transform* t, const glm::vec3& v){ t->position = v; })
    );

    luaReader.new_usertype<CameraController>(
        "Camera",
        sol::constructors<CameraController()>(),
        "position", sol::property([](CameraController* cam){ return cam->position; }, [](CameraController* cam, const glm::vec3& v){ cam->position = v; })
    );

    std::string folderPath = "/Users/hayyan/Desktop/Repos/CPPEngine/Rewrite/src/Assets/Scripts";
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::ifstream file(entry.path());

        if(!file.is_open()){
            std::cout << "[Sctript Storage] Failed to open file \n";
            continue;
        }

        sol::protected_function script = luaReader.load_file(entry.path().string());
        std::cout << "[Script Storage] Loading script {" << entry.path().stem().string() << "} \n";
        scripts.emplace(entry.path().stem().string(), script);
    }
}

sol::protected_function ScriptStorage::GetScript(std::string name){
    if(scripts.count(name) > 0){
        return scripts.at(name);
    }
    else{
        return scripts[0];
    }
}