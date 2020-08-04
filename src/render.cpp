#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/render.hpp"
#include "Graphics/gmath.hpp"

using namespace gmh;

Visual::Visual(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
}

Visual::Visual(const Visual& obj){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    VBO_DATA = obj.VBO_DATA;
    IBO_DATA = obj.IBO_DATA;
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ARRAY_BUFFER, VBO_DATA.size()*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(7*sizeof(float)));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO_DATA.size()*sizeof(unsigned int), IBO_DATA.data(), GL_STATIC_DRAW);
}

Visual::Visual(Visual&& obj){
    VAO = obj.VAO;
    VBO = obj.VBO;
    IBO = obj.IBO;
    obj.VAO = 0;
    obj.VBO = 0;
    obj.IBO = 0;
    VBO_DATA = std::move(obj.VBO_DATA);
    IBO_DATA = std::move(obj.IBO_DATA);
}

Visual::~Visual(){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
}

void Visual::reload(){
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, VBO_DATA.size()*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
}

void Visual::set_color(const float r, const float g, const float b, const float a){
    for(unsigned int i = 0; i < VBO_DATA.size(); i+=STRIDE){
        VBO_DATA[i + RED] = r;
        VBO_DATA[i + GREEN] = g;
        VBO_DATA[i + BLUE] = b;
        VBO_DATA[i + ALPHA] = a;
    }
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

Visual& Visual::operator=(const Visual& obj){
    VBO_DATA = obj.VBO_DATA;
    IBO_DATA = obj.IBO_DATA;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, VBO_DATA.size()*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO_DATA.size()*sizeof(unsigned int), IBO_DATA.data(), GL_STATIC_DRAW);
    return *this;
}

Visual& Visual::operator=(Visual&& obj){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
    VAO = obj.VAO;
    VBO = obj.VBO;
    IBO = obj.IBO;
    obj.VAO = 0;
    obj.VBO = 0;
    obj.IBO = 0;
    VBO_DATA = std::move(obj.VBO_DATA);
    IBO_DATA = std::move(obj.IBO_DATA);
    return *this;
}

Surface::Surface(){
    VBO_DATA = {0, 0, 0, 1, 1, 1, 1, 0, 0,
                1, 0, 0, 1, 1, 1, 1, 0, 0,
                0, 1, 0, 1, 1, 1, 1, 0, 0};
    IBO_DATA = {0, 1, 2};
    glBufferData(GL_ARRAY_BUFFER, 27*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(7*sizeof(float)));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int), IBO_DATA.data(), GL_STATIC_DRAW);
}

Surface::Surface(std::vector<Point> vert): Polygon(vert){
    VBO_DATA.resize(STRIDE*vertices.size());
    for(unsigned int i = 0; i < VBO_DATA.size(); i+=STRIDE){
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
    IBO_DATA.resize(3*(vertices.size() - 2));
    for(unsigned int i = 0; i < IBO_DATA.size(); i+=3){
        IBO_DATA[i] = 0;
        IBO_DATA[i + 1] = i/3 + 1;
        IBO_DATA[i + 2] = i/3 + 2;
    }
    glBufferData(GL_ARRAY_BUFFER, VBO_DATA.size()*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(7*sizeof(float)));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO_DATA.size()*sizeof(unsigned int), IBO_DATA.data(), GL_STATIC_DRAW);
}

Polygon Surface::local(){
    std::vector<Point> vert(vertices.size());
    for(unsigned int i = 0; i < vertices.size(); i++){
        vert[i].pos = glm::inverse(model)*glm::vec4(vertices[i]->pos, 1.0);
    }
    return Polygon(vert);
}

Solid::Solid(){
    VBO_DATA = {0, 0, 0, 1, 1, 1, 1, 0, 0,
                1, 0, 0, 1, 1, 1, 1, 0, 0,
                0, 1, 0, 1, 1, 1, 1, 0, 0,
                0, 0, 1, 1, 1, 1, 1, 0, 0};
    IBO_DATA = {0, 1, 2,
                0, 2, 3,
                0, 3, 1,
                3, 2, 1};
    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(7*sizeof(float)));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*sizeof(unsigned int), IBO_DATA.data(), GL_STATIC_DRAW);
}

Solid::Solid(std::vector<Point> vert): Polyhedron(vert){
    VBO_DATA.resize(STRIDE*vertices.size());
    for(unsigned int i = 0; i < VBO_DATA.size(); i+=STRIDE){
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
    size_t indices = 0;
    for(std::shared_ptr<Polygon> face: faces){
        indices += 3*(face->vertices.size() - 2);
    }
    IBO_DATA.resize(indices);
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
            IBO_DATA[j++] = static_cast<unsigned int>(std::distance(vertices.begin(), it1));
            IBO_DATA[j++] = static_cast<unsigned int>(std::distance(vertices.begin(), it2));
            IBO_DATA[j++] = static_cast<unsigned int>(std::distance(vertices.begin(), it3));
        }
    }
    glBufferData(GL_ARRAY_BUFFER, VBO_DATA.size()*sizeof(float), VBO_DATA.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, STRIDE*sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, STRIDE*sizeof(float), reinterpret_cast<void*>(7*sizeof(float)));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO_DATA.size()*sizeof(unsigned int), IBO_DATA.data(), GL_STATIC_DRAW);
}

Polyhedron Solid::local(){
    std::vector<Point> vert(vertices.size());
    for(unsigned int i = 0; i < vertices.size(); i++){
        vert[i].pos = glm::inverse(model)*glm::vec4(vertices[i]->pos, 1.0);
    }
    return Polyhedron(vert);
}
