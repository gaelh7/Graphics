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
    // std::cout << &glGenBuffers << std::endl;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
};

void Surface::render() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VBO_DATA), &VBO_DATA, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3*sizeof(float), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IBO_DATA), &IBO_DATA, GL_STATIC_DRAW);
};