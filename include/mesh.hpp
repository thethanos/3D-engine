#pragma once

#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "shader_program.hpp"

struct vertex
{
    glm::vec3 position;         
    glm::vec3 normal;
    glm::vec2 tex_coords;   //texture coordinates
};

struct texture
{
    GLuint id;
    GLuint type;
    std::string name;
};

class Mesh
{
public:
    Mesh(std::vector<vertex>& vertices, std::vector<GLuint>& indices, std::vector<texture>& textures);

public:
    void Draw(ShaderProgram& sp);

private:
    void load_to_gpu();

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<texture> textures;
};