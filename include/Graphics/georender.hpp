#pragma once

#include <glad/glad.h>
#include "Graphics/geometry.hpp"

namespace gmh {
    class Visual;
    class Surface;
    class Solid;

    enum Attrib {
        PosX, PosY, PosZ, RED, GREEN, BLUE, ALPHA, TexU, TexV, STRIDE
    };

    class Visual {
        protected:
            unsigned int VAO, VBO, IBO;
            std::vector<float> VBO_DATA;
            std::vector<unsigned int> IBO_DATA;
        public:
            Visual();
            Visual(const Visual& obj);
            Visual(Visual&& obj);
            ~Visual();
            void reload();
            void set_color(const float r, const float g, const float b, const float a);
            void vertex_color(const unsigned int vertex, const float r, const float g, const float b, const float a);
            void tex_coord(const unsigned int vertex, const float x, const float y);
            inline void render() const {
                glDrawElements(GL_TRIANGLES, static_cast<int>(IBO_DATA.size()), GL_UNSIGNED_INT, nullptr);
            }
            inline void bind() const {
                glBindVertexArray(VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            }
            Visual& operator=(const Visual& obj);
            Visual& operator=(Visual&& obj);
            void VBO_PRINT() const {
                std::cout << "VBO ID: " << VBO << '\n';
                for(unsigned int i = 0; i < VBO_DATA.size(); i++){
                    std::cout << VBO_DATA[i] << "\t";
                    if(i%STRIDE==STRIDE-1) std::cout << std::endl;
                }
            }
            void IBO_PRINT() const {
                std::cout << "IBO ID: " << IBO << '\n';
                for(unsigned int i = 0; i < IBO_DATA.size(); i+=3){
                    std::cout << IBO_DATA[i] << ", " << IBO_DATA[i+1] << ", " << IBO_DATA[i + 2] << std::endl;
                }
            }
    };

    class Surface: public Visual, public Polygon {
        public:
            Surface();
            template<typename... Points>
            Surface(Points... args): Surface(std::vector<Point>{args...}){};
            Surface(std::vector<Point> vert);
            Polygon local();
    };

    class Solid: public Visual, public Polyhedron {
        public:
            Solid();
            template<typename... Points>
            Solid(Points... args): Solid(std::vector<Point>{args...}){};
            Solid(std::vector<Point> vert);
            Polyhedron local();
    };
}
