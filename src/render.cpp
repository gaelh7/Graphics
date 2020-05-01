#include <glad/glad.h>
#include "Graphics/render.hpp"

static double norm(const xt::xtensor_fixed<double, xt::xshape<3>> &vec){
    double mag = 0;
    for(double i: vec) mag += i*i;
    return sqrt(mag);
};

Surface::Surface(xt::xtensor_fixed<double, xt::xshape<3>> vel, xt::xtensor_fixed<double, xt::xshape<3>> acc, std::vector<Point> vert): Polygon(vert), vel(vel), acc(acc) {
    VBO_DATA = std::make_unique<double[]>(3*vertices.size());
    for(unsigned int i = 0; i < 3*vertices.size(); i++)
        VBO_DATA[i] = vertices[i/3].pos[i%3];
    IBO_DATA = std::make_unique<unsigned int[]>(3*(vertices.size() - 2));
    for(unsigned int i = 0; i < 3*(vertices.size() - 2); i+=3){
        IBO_DATA[i] = 0;
        IBO_DATA[i + 1] = i/3 + 1;
        IBO_DATA[i + 2] = i/3 + 2;
    }
    int VBO_RESET;
    int IBO_RESET;
    int VAO_RESET;
    GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET));
    GLCALL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &IBO_RESET));
    GLCALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &VAO_RESET));

    GLCALL(glGenVertexArrays(1, &VAO));
    GLCALL(glBindVertexArray(VAO));

    GLCALL(glGenBuffers(1, &VBO));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 3*vertices.size()*sizeof(double), VBO_DATA.get(), GL_STATIC_DRAW));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_DOUBLE, false, 3*sizeof(double), nullptr));

    GLCALL(glGenBuffers(1, &IBO));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*(vertices.size() - 2)*sizeof(unsigned int), IBO_DATA.get(), GL_STATIC_DRAW));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, IBO_RESET));
    GLCALL(glBindVertexArray(VAO_RESET));
}

Surface::~Surface(){
    GLCALL(glDeleteBuffers(1, &VBO));
    GLCALL(glDeleteBuffers(1, &IBO));
}

void Surface::update(double dt){
    vel += acc*dt;
    for(unsigned int i = 0; i < 3*vertices.size(); i++){
        VBO_DATA[i] += vel[i%3];
        if(i%3 == 0){
            vertices[i/3].pos += vel;
            edges[i/3].vertices[0].pos += vel;
            edges[i/3].vertices[1].pos += vel;
        }
    }

    int VBO_RESET;
    GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 3*vertices.size()*sizeof(double), VBO_DATA.get(), GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
}

// void Surface::rotate(xt::xtensor_fixed<double, xt::xshape<3>> axis, double theta, xt::xtensor_fixed<double, xt::xshape<3>> p){
//     axis /= norm(axis);

// }

// inline void Surface::bind() const{
//     GLCALL(glBindVertexArray(VAO));
//     GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
// };

// inline void Surface::render() const {
//     GLCALL(glDrawElements(GL_TRIANGLES, 3*(vertices.size() - 2), GL_UNSIGNED_INT, nullptr));
// };
