#include "glad/glad.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <stdexcept>

#include "stb_image.h"

#include "model.hpp"


void Model::Load(const std::string& path, const std::string& name)
{
    model_path = path;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path + "/" + name, 
    aiProcess_Triangulate | 
    aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error("Failed to load model " + name);

    process_node(scene->mRootNode, scene);
}

void Model::Draw(ShaderProgram& sp) 
{
    for (auto mesh : meshes)
        mesh.Draw(sp);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    for (GLuint i(0); i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for (GLuint i(0); i < node->mNumChildren; ++i)
        process_node(node->mChildren[i], scene);
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{   
    bool hasNormals = mesh->HasNormals();
    bool hasTexture = bool(mesh->mTextureCoords[0]);

    std::vector<vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    for (GLuint i(0); i < mesh->mNumVertices; i++)
    {
        vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if(hasNormals)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if(hasTexture)
        {
            vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    std::vector<GLuint> indices;
    indices.reserve(mesh->mNumFaces);
    for (GLuint i(0); i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j(0); j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::vector<texture> textures = load_materials(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);

    return Mesh{vertices, indices, textures};
}

std::vector<texture> Model::load_materials(aiMaterial* material, aiTextureType type)
{
    std::vector<texture> textures;
    for (GLuint i(0); i < material->GetTextureCount(type); ++i)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        
        std::string path = str.C_Str();

        if (texture_cache.count(path))
        {
            textures.push_back(texture_cache[path]);
            continue;
        }

        texture texture;
        texture.id = load_texture_from_file(path);
        texture.type = type;
        texture.name = path;
        textures.push_back(texture);
        texture_cache[path] = texture;
    }

    return textures;
}

GLuint Model::load_texture_from_file(const std::string& name)
{
    stbi_set_flip_vertically_on_load(true); 
    std::string path = model_path + "/" + name;

    GLuint id;
    glGenTextures(1, &id);

    int width(0), height(0), nrComponents(0);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
        throw std::runtime_error("Failed to load texture");

    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return id;
}