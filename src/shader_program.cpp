#include <fstream>

#include "shader_program.hpp"
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram(const std::string& vertex, const std::string& fragment, const std::string& path)
{
    GLuint vID = load_and_compile(path + "/" + vertex, GL_VERTEX_SHADER);
    GLuint fID = load_and_compile(path + "/" + fragment, GL_FRAGMENT_SHADER);

    program_id = glCreateProgram();
    glAttachShader(program_id, vID);
    glAttachShader(program_id, fID);
    glLinkProgram(program_id);

    GLint success(0);
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
        throw std::runtime_error("Failed to link shader program.");

    glDeleteShader(vID);
    glDeleteShader(fID);
}

void ShaderProgram::set_uniform1i(const std::string& name, GLuint value)
{
    GLuint location = glGetUniformLocation(program_id, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::set_uniform3fv(const std::string& name, const glm::vec3& vector)
{
    GLuint location = glGetUniformLocation(program_id, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void ShaderProgram::set_uniform_matrix4fv(const std::string& name, const glm::mat4& matrix)
{
    GLuint location = glGetUniformLocation(program_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint ShaderProgram::load_and_compile(const std::string& path, GLuint type)
{
    std::ifstream fst(path);
    if (!fst.is_open())
        throw std::runtime_error("Failed to open shader file " + path);

    std::string source_code{std::istreambuf_iterator<char>(fst), std::istreambuf_iterator<char>()};
    fst.close();
    
    GLuint id = glCreateShader(type);
    const char* source = source_code.c_str();
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);

    GLint success(0);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
        throw std::runtime_error("Failed to compile shader " + path);

    return id;
}