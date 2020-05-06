#include <glad/glad.h>
#include "Graphics/render.hpp"

Surface::Surface(std::vector<Point> vert): Polygon(vert){
    VBO_DATA = new float[stride*vertices.size()];
    for(unsigned int i = 0; i < stride*vertices.size(); i+=stride){
        VBO_DATA[i] = vertices[i/stride].pos.x;
        VBO_DATA[i + 1] = vertices[i/stride].pos.y;
        VBO_DATA[i + 2] = vertices[i/stride].pos.z;
        VBO_DATA[i + 3] = 1;
        VBO_DATA[i + 4] = 1;
        VBO_DATA[i + 5] = 1;
        VBO_DATA[i + 6] = 1;
        VBO_DATA[i + 7] = 0;
        VBO_DATA[i + 8] = 0;
    }
    IBO_DATA = new unsigned int[3*(vertices.size() - 2)];
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
    GLCALL(glBufferData(GL_ARRAY_BUFFER, stride*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, stride*sizeof(float), nullptr));
    GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, false, stride*sizeof(float), (const void*)(3*sizeof(float))));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, false, stride*sizeof(float), (const void*)(7*sizeof(float))));

    GLCALL(glGenBuffers(1, &IBO));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*(vertices.size() - 2)*sizeof(unsigned int), IBO_DATA, GL_STATIC_DRAW));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, IBO_RESET));
    GLCALL(glBindVertexArray(VAO_RESET));
}

Surface::~Surface(){
    GLCALL(glDeleteBuffers(1, &VBO));
    GLCALL(glDeleteBuffers(1, &IBO));
    GLCALL(glDeleteVertexArrays(1, &VAO));
    delete[] VBO_DATA;
    delete[] IBO_DATA;
}

Surface Surface::local(){
    Surface out(this->vertices);
    out.transform(glm::inverse(model));
    return Surface(out.vertices);
}

void Surface::transform(){
    model = trans_mat*model;
    for(unsigned int i = 0; i < vertices.size(); i++){
        pos = (trans_mat * glm::vec4(pos, 1)).xyz;
        vertices[i].pos = (trans_mat * glm::vec4(vertices[i].pos, 1)).xyz;
        edges[i].vertices[0].pos = (trans_mat * glm::vec4(edges[i].vertices[0].pos, 1)).xyz;
        edges[i].vertices[1].pos = (trans_mat * glm::vec4(edges[i].vertices[1].pos, 1)).xyz;
    }
}

void Surface::transform(glm::mat4 mat){
    model = mat*model;
    for(unsigned int i = 0; i < vertices.size(); i++){
        vertices[i].pos = (mat * glm::vec4(vertices[i].pos, 1)).xyz;
        edges[i].vertices[0].pos = (mat * glm::vec4(edges[i].vertices[0].pos, 1)).xyz;
        edges[i].vertices[1].pos = (mat * glm::vec4(edges[i].vertices[1].pos, 1)).xyz;
    }
}

void Surface::reload(){
    int VBO_RESET;
    GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, stride*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
}

void Surface::set_color(const float r, const float g, const float b, const float a){
    for(unsigned int i = 0; i < stride*vertices.size(); i+=stride){
        VBO_DATA[i + 3] = r;
        VBO_DATA[i + 4] = g;
        VBO_DATA[i + 5] = b;
        VBO_DATA[i + 6] = a;
    }
}

void Surface::vertex_color(const unsigned int vertex, const float r, const float g, const float b, const float a){
    VBO_DATA[stride*vertex + 3] = r;
    VBO_DATA[stride*vertex + 4] = g;
    VBO_DATA[stride*vertex + 5] = b;
    VBO_DATA[stride*vertex + 6] = a;
}

void Surface::tex_coord(const unsigned int vertex, const float x, const float y){
    VBO_DATA[stride*vertex + 7] = x;
    VBO_DATA[stride*vertex + 8] = y;
}

Solid::Solid(std::vector<Point> vert): Polyhedron(vert){
    VBO_DATA = new float[stride*vertices.size()];
    for(unsigned int i = 0; i < stride*vertices.size(); i+=stride){
        VBO_DATA[i] = vertices[i/stride].pos.x;
        VBO_DATA[i + 1] = vertices[i/stride].pos.y;
        VBO_DATA[i + 2] = vertices[i/stride].pos.z;
        VBO_DATA[i + 3] = 1;
        VBO_DATA[i + 4] = 1;
        VBO_DATA[i + 5] = 1;
        VBO_DATA[i + 6] = 1;
        VBO_DATA[i + 7] = 0;
        VBO_DATA[i + 8] = 0;
    }
    indices = 0;
    for(Polygon face: faces){
        indices += 3*(face.vertices.size() - 2);
    }
    IBO_DATA = new unsigned int[indices];
    int j = 0;
    for(Polygon face: faces){
        for(unsigned int i = 0; i < (face.vertices.size() - 2); i++){
            auto it1 = std::find_if(vertices.begin(), vertices.end(), [&face, i](Point p){
                return face.vertices[0].equals(p);
            });
            auto it2 = std::find_if(vertices.begin(), vertices.end(), [&face, i](Point p){
                return face.vertices[i + 1].equals(p);
            });
            auto it3 = std::find_if(vertices.begin(), vertices.end(), [&face, i](Point p){
                return face.vertices[i + 2].equals(p);
            });
            IBO_DATA[j++] = std::distance(vertices.begin(), it1);
            IBO_DATA[j++] = std::distance(vertices.begin(), it2);
            IBO_DATA[j++] = std::distance(vertices.begin(), it3);
        }
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
    GLCALL(glBufferData(GL_ARRAY_BUFFER, stride*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, stride*sizeof(float), nullptr));
    GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, false, stride*sizeof(float), (const void*)(3*sizeof(float))));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, false, stride*sizeof(float), (const void*)(7*sizeof(float))));

    GLCALL(glGenBuffers(1, &IBO));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices*sizeof(unsigned int), IBO_DATA, GL_STATIC_DRAW));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, IBO_RESET));
    GLCALL(glBindVertexArray(VAO_RESET));

}

Solid::~Solid(){
    GLCALL(glDeleteBuffers(1, &VBO));
    GLCALL(glDeleteBuffers(1, &IBO));
    GLCALL(glDeleteVertexArrays(1, &VAO));
    delete[] VBO_DATA;
    delete[] IBO_DATA;
}

void Solid::reload(){
    int VBO_RESET;
    GLCALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, stride*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET));
}

void Solid::set_color(const float r, const float g, const float b, const float a){
    for(unsigned int i = 0; i < stride*vertices.size(); i+=stride){
        VBO_DATA[i + 3] = r;
        VBO_DATA[i + 4] = g;
        VBO_DATA[i + 5] = b;
        VBO_DATA[i + 6] = a;
    }
}

void Solid::vertex_color(const unsigned int vertex, const float r, const float g, const float b, const float a){
    VBO_DATA[stride*vertex + 3] = r;
    VBO_DATA[stride*vertex + 4] = g;
    VBO_DATA[stride*vertex + 5] = b;
    VBO_DATA[stride*vertex + 6] = a;
}

void Solid::tex_coord(const unsigned int vertex, const float x, const float y){
    VBO_DATA[stride*vertex + 7] = x;
    VBO_DATA[stride*vertex + 8] = y;
}
