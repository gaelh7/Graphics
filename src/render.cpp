#include <glad/glad.h>
#include "Graphics/render.hpp"

Surface::Surface(std::vector<Point> vert): Polygon(vert) {
    VBO_DATA = std::make_unique<double[]>(stride*vertices.size());
    for(unsigned int i = 0; i < stride*vertices.size(); i+=stride){
        VBO_DATA[i] = vertices[i/stride].pos.x;
        VBO_DATA[i + 1] = vertices[i/stride].pos.y;
        VBO_DATA[i + 2] = vertices[i/stride].pos.z;
        VBO_DATA[i + 3] = 1;
        VBO_DATA[i + 4] = 1;
        VBO_DATA[i + 5] = 1;
        VBO_DATA[i + 6] = 1;
    }
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
    GLCALL(glBufferData(GL_ARRAY_BUFFER, stride*vertices.size()*sizeof(double), VBO_DATA.get(), GL_STATIC_DRAW));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(0, 3, GL_DOUBLE, false, stride*sizeof(double), nullptr));
    GLCALL(glVertexAttribPointer(1, 4, GL_DOUBLE, false, stride*sizeof(double), (const void*)(3*sizeof(double))));
    GLCALL(glVertexAttribPointer(2, 2, GL_DOUBLE, false, stride*sizeof(double), (const void*)(7*sizeof(double))));

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
    GLCALL(glDeleteVertexArrays(1, &VAO));
}

void Surface::transform(){
    tot_change = trans_mat*tot_change;
    for(unsigned int i = 0; i < vertices.size(); i++){
        vertices[i].pos = (trans_mat * glm::dvec4(vertices[i].pos, 1)).xyz();
        edges[i].vertices[0].pos = (trans_mat * glm::dvec4(edges[i].vertices[0].pos, 1)).xyz();
        edges[i].vertices[1].pos = (trans_mat * glm::dvec4(edges[i].vertices[1].pos, 1)).xyz();
    }
}

void Surface::reload() {
    int VBO_RESET;
    GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, stride*vertices.size()*sizeof(double), VBO_DATA.get(), GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
}

void Surface::set_color(const double r, const double g, const double b, const double a){
    for(unsigned int i = 0; i < stride*vertices.size(); i+=stride){
        VBO_DATA[i + 3] = r;
        VBO_DATA[i + 4] = g;
        VBO_DATA[i + 5] = b;
        VBO_DATA[i + 6] = a;
    }
}

void Surface::vertex_color(const unsigned int vertex, const double r, const double g, const double b, const double a) {
    VBO_DATA[stride*vertex + 3] = r;
    VBO_DATA[stride*vertex + 4] = g;
    VBO_DATA[stride*vertex + 5] = b;
    VBO_DATA[stride*vertex + 6] = a;
}

void Surface::tex_coord(const unsigned int vertex, const double x, const double y){
    VBO_DATA[stride*vertex + 7] = x;
    VBO_DATA[stride*vertex + 8] = y;
}
