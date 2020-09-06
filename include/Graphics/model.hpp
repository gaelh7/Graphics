#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <assimp/scene.h>
#include "Graphics/texture.hpp"
#include "Graphics/shader.hpp"

namespace gmh {
    struct Vertex {
        glm::vec3 position, normal;
        glm::vec2 texCoords;
        glm::vec3 tangent, bitangent;
    };

    class Mesh {
        unsigned int VAO, VBO, IBO;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture*> textures;
        public:
            Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> text);
            void render(const Shader& program) const;
    };

    class Model {
        std::deque<Texture> loaded;
        std::vector<Mesh> meshes;
        std::string path;
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        void loadMaterialTextures(std::vector<Texture*>& textures, aiMaterial *mat, aiTextureType type);
        public:
            Model(const char* filepath);
            void render(const Shader& program) const;
    };
}
