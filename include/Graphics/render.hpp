#pragma once

#include "Graphics/geometry.hpp"
#include "Graphics/log.hpp"

class Surface: public Polygon{
    private:
        static constexpr unsigned char stride = 9;
        unsigned int VAO, VBO, IBO;
        std::unique_ptr<double[]> VBO_DATA;
        std::unique_ptr<unsigned int[]> IBO_DATA;
    public:
        xt::xtensor_fixed<double, xt::xshape<3>> vel;
        xt::xtensor_fixed<double, xt::xshape<3>> acc;
        template<typename... Points>
        Surface(Points... args): Surface(std::vector<Point>{args...}){};
        Surface(std::vector<Point> vert);
        ~Surface();
        void update(const double dt);
        void set_color(const double r, const double g, const double b, const double a);
        void vertex_color(const unsigned int vertex, const double r, const double g, const double b, const double a);
        // void rotate(xt::xtensor_fixed<double, xt::xshape<3>> axis, double theta, xt::xtensor_fixed<double, xt::xshape<3>> p);
        inline void bind() const{
            GLCALL(glBindVertexArray(VAO));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
        }
        inline void render() const{
            GLCALL(glDrawElements(GL_TRIANGLES, 3*(vertices.size() - 2), GL_UNSIGNED_INT, nullptr));
        }
        void PRINT_VBOSET(){

        }
};

class Solid: public Polyhedron{};
