#pragma once

#include <assimp/scene.h>

#include <map>

#include "mesh.hpp"

class Model
{
public:
    void Load(const std::string& path, const std::string& name);
    void Draw(ShaderProgram& sp);

private:
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    
    std::vector<texture> load_materials(aiMaterial* material, aiTextureType type);
    GLuint load_texture_from_file(const std::string& name);

private:
    std::string model_path;
    std::vector<Mesh> meshes;
    std::map<std::string, texture> texture_cache;
};