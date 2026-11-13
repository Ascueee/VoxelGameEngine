#include <ScriptStorage.hpp>

sol::state ScriptStorage::luaReader;
std::unordered_map<std::string, Script> ScriptStorage::scripts;
ScriptStorage::ScriptStorage(){}

void ScriptStorage::Load(){
    luaReader.open_libraries(sol::lib::base);

    //Data types
    luaReader.new_usertype<Entity>(
        "Entity",
        sol::constructors<Entity()>(),
        "GetName",[](Entity& e){return e.GetName();},
        "AddTransform",[](Entity& e){e.AddPackage<Transform>();},
        "transform", sol::property([](Entity& e){ return e.GetPackage<Transform>();}),
        "AddMeshController",[](Entity& e){e.AddPackage<ModelController>();},
        "meshController", sol::property([](Entity& e){ return e.GetPackage<ModelController>();}),
        "camera", sol::property([](Entity& e){ return e.GetPackage<CameraController>();})
    );

    luaReader.new_usertype<EntityStorage>(
        "EntityManager",
        sol::constructors<EntityStorage()>()
    );


    luaReader.new_usertype<ModelStorage>(
        "ModelStorage",
        sol::constructors<ModelStorage()>(),
        "GetModel",[](ModelStorage& model, std::string name)-> Model* {return model.GetModel(name);}
    );

    luaReader.new_usertype<glm::vec3>(
        "Vector3",
        sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", sol::property([](glm::vec3& v){ return v.x; }, [](glm::vec3& v, float f){ v.x = f; }),
        "y", sol::property([](glm::vec3& v){ return v.y; }, [](glm::vec3& v, float f){ v.y = f; }),
        "z", sol::property([](glm::vec3& v){ return v.z; }, [](glm::vec3& v, float f){ v.z = f; }),
        sol::meta_function::addition,    [](const glm::vec3& a, const glm::vec3& b){ return a + b; }
    );

    luaReader.new_usertype<Model>(
        "Model",
        sol::constructors<Model()>(),
        "AddMesh", &Model::AddMesh,
        "GetMesh", &Model::GetMesh
    );

    luaReader.new_usertype<Transform>(
        "Transform",
        sol::constructors<Transform()>(),
        "position", sol::property([](Transform* t){ return t->position; }, [](Transform* t, const glm::vec3& v){ t->position = v; })
    );

    luaReader.new_usertype<ModelController>(
        "ModelController",
        sol::constructors<ModelController()>(),
        "model", sol::property([](ModelController& controller) -> Model* { return controller.model;}, [](ModelController& controller, Model* model) {controller.model = model;})
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

        Script script;
        luaReader.script_file(entry.path().string());
        sol::protected_function luaScriptLoad = luaReader["Load"];
        script.SetScriptLoad(luaScriptLoad);
        sol::protected_function luaScriptUpdate = luaReader["Update"];
        script.SetScriptUpdate(luaScriptUpdate);

        std::cout << "[Script Storage] Loading script {" << entry.path().stem().string() << "} \n";
        scripts.emplace(entry.path().stem().string(), script);
    }
}

Script* ScriptStorage::GetScript(std::string name){
    if(scripts.count(name) > 0){
        return &scripts.at(name);
    }
    else{
        return &scripts[0];
    }
}