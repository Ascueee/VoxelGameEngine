#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once
class Shader{
    public:
        
        Shader(const char* vertexPath, const char* fragmentPath);
        void Use();
        void setBool(const std::string &name, bool value) const;  
        void setInt(const std::string &name, int value) const;   
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setMat4Array(const std::string& name, const std::vector<glm::mat4>& matrices);

    private:
        unsigned int id;
        void CheckError(unsigned int shader, std::string type);
    
};