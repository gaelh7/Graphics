#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/render.hpp"
#include "Graphics/gmath.hpp"

using namespace gmh;

Visual::Visual(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

Visual::~Visual(){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
    delete[] VBO_DATA;
    delete[] IBO_DATA;
}

void Visual::vertex_color(const unsigned int vertex, const float r, const float g, const float b, const float a){
    VBO_DATA[STRIDE*vertex + RED] = r;
    VBO_DATA[STRIDE*vertex + GREEN] = g;
    VBO_DATA[STRIDE*vertex + BLUE] = b;
    VBO_DATA[STRIDE*vertex + ALPHA] = a;
}

void Visual::tex_coord(const unsigned int vertex, const float x, const float y){
    VBO_DATA[STRIDE*vertex + TexU] = x;
    VBO_DATA[STRIDE*vertex + TexV] = y;
}

Surface::Surface(std::vector<Point> vert): Visual(), Polygon(vert){
    VBO_DATA = new float[STRIDE*vertices.size()];
    for(unsigned int i = 0; i < STRIDE*vertices.size(); i+=STRIDE){
        VBO_DATA[i + PosX] = vertices[i/STRIDE]->pos.x;
        VBO_DATA[i + PosY] = vertices[i/STRIDE]->pos.y;
        VBO_DATA[i + PosZ] = vertices[i/STRIDE]->pos.z;
        VBO_DATA[i + RED] = 1;
        VBO_DATA[i + GREEN] = 1;
        VBO_DATA[i + BLUE] = 1;
        VBO_DATA[i + ALPHA] = 1;
        VBO_DATA[i + TexU] = 0;
        VBO_DATA[i + TexV] = 0;
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
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &IBO_RESET);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &VAO_RESET);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, STRIDE*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), (void*)(7*sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*(vertices.size() - 2)*sizeof(unsigned int), IBO_DATA, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET);
    glBindBuffer(GL_ARRAY_BUFFER, IBO_RESET);
    glBindVertexArray(VAO_RESET);
}

Polygon Surface::local(){
    std::vector<Point> vert(vertices.size());
    for(unsigned int i = 0; i < vertices.size(); i++){
        vert[i].pos = glm::inverse(model)*glm::vec4(vertices[i]->pos, 1.0);
    }
    return Polygon(vert);
}

void Surface::update(float dt){
    model = glm::translate(model, dt*vel);
    pos += dt*vel;
    for(std::shared_ptr<Point> p: vertices)
        p->pos += dt*vel;
}

void Surface::reload(){
    int VBO_RESET;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, STRIDE*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET);
}

void Surface::set_color(const float r, const float g, const float b, const float a){
    for(unsigned int i = 0; i < STRIDE*vertices.size(); i+=STRIDE){
        VBO_DATA[i + RED] = r;
        VBO_DATA[i + GREEN] = g;
        VBO_DATA[i + BLUE] = b;
        VBO_DATA[i + ALPHA] = a;
    }
}

Solid::Solid(std::vector<Point> vert): Visual(), Polyhedron(vert){
    VBO_DATA = new float[STRIDE*vertices.size()];
    for(unsigned int i = 0; i < STRIDE*vertices.size(); i+=STRIDE){
        VBO_DATA[i + PosX] = vertices[i/STRIDE]->pos.x;
        VBO_DATA[i + PosY] = vertices[i/STRIDE]->pos.y;
        VBO_DATA[i + PosZ] = vertices[i/STRIDE]->pos.z;
        VBO_DATA[i + RED] = 1;
        VBO_DATA[i + GREEN] = 1;
        VBO_DATA[i + BLUE] = 1;
        VBO_DATA[i + ALPHA] = 1;
        VBO_DATA[i + TexU] = 0;
        VBO_DATA[i + TexV] = 0;
    }
    indices = 0;
    for(std::shared_ptr<Polygon> face: faces){
        indices += (unsigned int)(3*(face->vertices.size() - 2));
    }
    IBO_DATA = new unsigned int[indices];
    int j = 0;
    for(std::shared_ptr<Polygon> face: faces){
        for(unsigned int i = 0; i < (face->vertices.size() - 2); i++){
            std::vector<std::shared_ptr<Point>>::iterator it1 = std::find_if(vertices.begin(), vertices.end(), [&face, i](std::shared_ptr<Point> p){
                return face->vertices[0]->equals(*p);
            });
            std::vector<std::shared_ptr<Point>>::iterator it2 = std::find_if(vertices.begin(), vertices.end(), [&face, i](std::shared_ptr<Point> p){
                return face->vertices[i + 1]->equals(*p);
            });
            std::vector<std::shared_ptr<Point>>::iterator it3 = std::find_if(vertices.begin(), vertices.end(), [&face, i](std::shared_ptr<Point> p){
                return face->vertices[i + 2]->equals(*p);
            });
            IBO_DATA[j++] = (unsigned int)std::distance(vertices.begin(), it1);
            IBO_DATA[j++] = (unsigned int)std::distance(vertices.begin(), it2);
            IBO_DATA[j++] = (unsigned int)std::distance(vertices.begin(), it3);
        }
    }
    int VBO_RESET;
    int IBO_RESET;
    int VAO_RESET;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &IBO_RESET);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &VAO_RESET);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, STRIDE*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), (void*)(7*sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices*sizeof(unsigned int), IBO_DATA, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET);
    glBindBuffer(GL_ARRAY_BUFFER, IBO_RESET);
    glBindVertexArray(VAO_RESET);
}

Polyhedron Solid::local(){
    std::vector<Point> vert(vertices.size());
    for(unsigned int i = 0; i < vertices.size(); i++){
        vert[i].pos = glm::inverse(model)*glm::vec4(vertices[i]->pos, 1.0);
    }
    return Polyhedron(vert);
}

void Solid::update(float dt){
    model = glm::translate(model, dt*vel);
    pos += dt*vel;
    for(std::shared_ptr<Point> p: vertices)
        p->pos += dt*vel;
}

void Solid::reload(){
    int VBO_RESET;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO_RESET);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, STRIDE*vertices.size()*sizeof(float), VBO_DATA, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_RESET);
}

void Solid::set_color(const float r, const float g, const float b, const float a){
    for(unsigned int i = 0; i < STRIDE*vertices.size(); i+=STRIDE){
        VBO_DATA[i + RED] = r;
        VBO_DATA[i + GREEN] = g;
        VBO_DATA[i + BLUE] = b;
        VBO_DATA[i + ALPHA] = a;
    }
}
