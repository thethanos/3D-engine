#include "mesh.hpp"

#include <iostream>

Mesh::Mesh(std::vector<vertex>& vertices, std::vector<GLuint>& indices, std::vector<texture>& textures)
{   
    this->vertices = std::move(vertices);
    this->indices  = std::move(indices);
    this->textures = std::move(textures);

    load_to_gpu();
}

void Mesh::Draw(ShaderProgram& sp)
{
    glBindVertexArray(VAO);

    for (GLuint i(0); i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        sp.set_uniform1i("diffuse_texture" + std::to_string(i), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::load_to_gpu()
{
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &EBO);
    glCreateBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, tex_coords)));

    glBindVertexArray(0);
}