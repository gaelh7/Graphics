#pragma once

#include <glad/glad.h>
#include "Graphics/geometry.hpp"

namespace gmh{
    class Visual;
    class Surface;
    class Solid;

    enum Attrib {
        PosX, PosY, PosZ, RED, GREEN, BLUE, ALPHA, TexU, TexV, STRIDE
    };

    class Visual {
        protected:
            unsigned int VAO, VBO, IBO;
            float* VBO_DATA;
            unsigned int* IBO_DATA;
        public:
            glm::mat4 model{1.0f};
            Visual();
            ~Visual();
            virtual void update(float dt) = 0;
            virtual void reload() = 0;
            virtual void set_color(const float r, const float g, const float b, const float a) = 0;
            void vertex_color(const unsigned int vertex, const float r, const float g, const float b, const float a);
            void tex_coord(const unsigned int vertex, const float x, const float y);
            inline virtual void render() const = 0;
            inline void bind() const {
                glBindVertexArray(VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            }
    };

    class Surface: public Visual, public Polygon {
        public:
            template<typename... Points>
            Surface(Points... args): Surface(std::vector<Point>{args...}){};
            Surface(std::vector<Point> vert);
            Polygon local();
            void update(float dt) override;
            void reload() override;
            void set_color(const float r, const float g, const float b, const float a) override;
            inline void render() const override {
                glDrawElements(GL_TRIANGLES, 3*((int)vertices.size() - 2), GL_UNSIGNED_INT, nullptr);
            }
            void VBO_PRINT() const {
                for(unsigned int i = 0; i < vertices.size(); i++){
                    for(unsigned int j = 0; j < STRIDE; j++)
                        std::cout << VBO_DATA[STRIDE*i + j] << "\t";
                    std::cout << std::endl;
                }
            }
    };

    class Solid: public Visual, public Polyhedron {
        private:
            unsigned int indices;
        public:
            template<typename... Points>
            Solid(Points... args): Solid(std::vector<Point>{args...}){};
            Solid(std::vector<Point> vert);
            Polyhedron local();
            void update(float dt) override;
            void reload() override;
            void set_color(const float r, const float g, const float b, const float a) override;
            inline void render() const override {
                glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
            }
            void VBO_PRINT() const {
                for(unsigned int i = 0; i < vertices.size(); i++){
                    for(unsigned int j = 0; j < STRIDE; j++)
                        std::cout << VBO_DATA[STRIDE*i + j] << "\t";
                    std::cout << std::endl;
                }
            }
            void IBO_PRINT() const {
                for(unsigned int i = 0; i < indices; i+=3){
                    std::cout << IBO_DATA[i] << ", " << IBO_DATA[i+1] << ", " << IBO_DATA[i + 2] << std::endl;
                }
            }
    };
}
