//
// Created by Kiril on 20.03.2022.
//

#ifndef KG3_SHADER_H
#define KG3_SHADER_H

#include <iostream>
#include "GL/glew.h"

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

class Shader
{
    unsigned int ID;

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    [[nodiscard]] int getLoc(const std::string& name) const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setFloats(const std::string &name, float val1, float val2) const;
    void setVec2(const std::string& name, glm::vec2 vec) const;

    void setFloats(const std::string &name, float val1, float val2, float val3) const;
    void setVec3(const std::string& name, glm::vec3 vec) const;

    void setFloats(const std::string &name, float val1, float val2, float val3, float val4) const;
    void setVec4(const std::string& name, glm::vec4 vec) const;

    void setMat4(const std::string& name, glm::mat4 mat4) const;
};

#endif //KG3_SHADER_H
