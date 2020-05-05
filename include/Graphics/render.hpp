#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/geometry.hpp"
#include "Graphics/log.hpp"

class Surface: public Polygon{
    private:
        static constexpr unsigned char stride = 9;
        unsigned int VAO, VBO, IBO;
        std::unique_ptr<double[]> VBO_DATA;
        std::unique_ptr<unsigned int[]> IBO_DATA;
    public:
        glm::mat4 trans_mat{1.0};
        glm::mat4 tot_change{1.0};
        template<typename... Points>
        Surface(Points... args): Surface(std::vector<Point>{args...}){};
        Surface(std::vector<Point> vert);
        ~Surface();
        void transform();
        void reload();
        void set_color(const double r, const double g, const double b, const double a);
        void vertex_color(const unsigned int vertex, const double r, const double g, const double b, const double a);
        void tex_coord(const unsigned int vertex, const double x, const double y);
        // void rotate(xt::xtensor_fixed<double, xt::xshape<3>> axis, double theta, xt::xtensor_fixed<double, xt::xshape<3>> p);
        inline void bind() const {
            GLCALL(glBindVertexArray(VAO));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
        }
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

class Solid: public Polyhedron{};
