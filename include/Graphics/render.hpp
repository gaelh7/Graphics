#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/geometry.hpp"
#include "Graphics/log.hpp"

class Visual {
    protected:
        static constexpr unsigned char stride = 9;
        unsigned int VAO, VBO, IBO;
        float* VBO_DATA;
        unsigned int* IBO_DATA;
    public:
        glm::mat4 model{1.0};
        Visual();
        ~Visual();
        void vertex_color(const unsigned int vertex, const float r, const float g, const float b, const float a);
        void tex_coord(const unsigned int vertex, const float x, const float y);
        inline void bind() const {
            GLCALL(glBindVertexArray(VAO));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
        }
};

class Surface: public Visual, public Polygon{
    public:
        template<typename... Points>
        Surface(Points... args): Surface(std::vector<Point>{args...}){};
        Surface(std::vector<Point> vert);
        Surface world();
        void reload();
        void set_color(const float r, const float g, const float b, const float a);
        inline void render() const {
            GLCALL(glDrawElements(GL_TRIANGLES, 3*(vertices.size() - 2), GL_UNSIGNED_INT, nullptr));
        }
        void VBO_PRINT() const {
            for(unsigned int i = 0; i < vertices.size(); i++){
                for(unsigned int j = 0; j < stride; j++)
                    std::cout << VBO_DATA[stride*i + j] << "\t";
                std::cout << std::endl;
            }
        }
};

class Solid: public Visual, public Polyhedron{
    private:
        unsigned int indices;
    public:
        template<typename... Points>
        Solid(Points... args): Solid(std::vector<Point>{args...}){};
        Solid(std::vector<Point> vert);
        Solid world();
        void reload();
        void set_color(const float r, const float g, const float b, const float a);
        inline void render() const {
            GLCALL(glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr));
        }
        void VBO_PRINT() const {
            for(unsigned int i = 0; i < vertices.size(); i++){
                for(unsigned int j = 0; j < stride; j++)
                    std::cout << VBO_DATA[stride*i + j] << "\t";
                std::cout << std::endl;
            }
        }
        void IBO_PRINT() const {
            for(unsigned int i = 0; i < indices; i+=3){
                std::cout << IBO_DATA[i] << ", " << IBO_DATA[i+1] << ", " << IBO_DATA[i + 2] << std::endl;
            }
        }
};
