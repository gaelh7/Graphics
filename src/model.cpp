#include "Graphics/model.hpp"
#include <iostream>
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace gmh;

Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> text): vertices(vert), indices(ind), textures(text) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

    glBindVertexArray(0);
}

void Mesh::render(const Shader& program) const {
    unsigned int numTypes[NUMTEXTYPES] = {1, 1, 1, 1, 1, 1, 1};
    for(unsigned int i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        program.SetUniformi((Texture::typeNames[textures[i]->type]+std::to_string(numTypes[i]++)).c_str(), i);
        textures[i]->bind(i);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::processNode(aiNode* node, const aiScene* scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        Vertex vertex;
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        vertex.texCoords = {0, 0};
        vertex.tangent = {0, 0, 0};
        vertex.bitangent = {0, 0, 0};
        if(mesh->mTextureCoords[0]){
            vertex.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            // vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
            // vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
        }
        vertices.push_back(vertex);
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);
    }
    if(mesh->mMaterialIndex >= 0){
        for(int i = DIFFUSE; i < NUMTEXTYPES; i++){
            loadMaterialTextures(textures, scene->mMaterials[mesh->mMaterialIndex], static_cast<aiTextureType>(i));
        }
    }

    return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(std::vector<Texture*>& textures, aiMaterial* mat, aiTextureType type){
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < loaded.size(); j++){
            if(loaded[j].path.substr(loaded[j].path.find_last_of('/') + 1, loaded[j].path.size()) == std::string(str.C_Str())){
                textures.push_back(&loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            loaded.emplace_back((path.substr(0, path.find_last_of('/')) + '/' + str.C_Str()).c_str(), static_cast<TextureType>(type));
            textures.push_back(&loaded.back());
        }
    }
}

Model::Model(const char* filepath): path(filepath) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::render(const Shader& program) const {
    for(Mesh mesh: meshes){
        mesh.render(program);
    }
}
