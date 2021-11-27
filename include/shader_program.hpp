#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <string>

#include "glm/glm.hpp"

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertex, const std::string& fragment, const std::string& path);

public:
    void use() { glUseProgram(program_id); }
    void set_uniform1i(const std::string& name, GLuint value);
    void set_uniform3fv(const std::string& name, const glm::vec3& vector);
    void set_uniform_matrix4fv(const std::string& name, const glm::mat4& matrix);

private:
    GLuint load_and_compile(const std::string& path, GLuint type);

private:
    GLuint program_id;
};