#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render.hpp"

Surface::Surface(std::vector<Point> vert): Polygon(vert){
    VBO_DATA = std::make_unique<double[]>(3*vertices.size());
    for(unsigned int i = 0; i < 3*vertices.size(); i++)
        VBO_DATA[i] = vertices[i/3].pos[i%3];
    IBO_DATA = std::make_unique<unsigned int[]>(3*(vertices.size() - 2));
    for(unsigned int i = 0; i < 3*(vertices.size() - 2); i+=3){
        IBO_DATA[i] = 0;
        IBO_DATA[i + 1] = i/3 + 1;
        IBO_DATA[i + 2] = i/3 + 2;
    }
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
};

Surface::~Surface(){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void Surface::bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3*vertices.size()*sizeof(double), VBO_DATA.get(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, false, 3*sizeof(double), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*(vertices.size() - 2)*sizeof(unsigned int), IBO_DATA.get(), GL_STATIC_DRAW);
};

void Surface::render() const {
    glDrawElements(GL_TRIANGLES, 3*(vertices.size() - 2), GL_UNSIGNED_INT, nullptr);
};
