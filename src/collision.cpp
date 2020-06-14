#include <glm/geometric.hpp>
#include "Graphics/collision.hpp"
#include "Graphics/gmath.hpp"

std::vector<std::vector<Physical>> CHandler::get_check() const {
    return combinations<Physical>(_tangible.begin(), _tangible.end(), 2);
}

CHandler::CHandler(){}

void CHandler::operator()() const {
    std::vector<std::vector<Physical>> objects = get_check();
    for(std::vector<Physical> v: objects){
        if(v[0].fixed && v[1].fixed) continue;
        if(Polygon* v0_surf = dynamic_cast<Polygon*>(v[0].obj)){
            if(v[1].obj->dist(*v0_surf) == 0 && glm::dot(v0_surf->vel - v[1].obj->vel, v0_surf->normVec()*v0_surf->sign_dist(*v[1].obj)) > 0)
                collision(v[0], v[1]);
        }
        else if(Polyhedron* v0_sol = dynamic_cast<Polyhedron*>(v[0].obj)){
            for(std::shared_ptr<Polygon> face: v0_sol->faces)
                if(v[1].obj->dist(*v0_sol) == 0 && glm::dot(v0_sol->vel - v[1].obj->vel, face->normVec()*face->sign_dist(*v[1].obj)) > 0){
                    collision(v[0], v[1]);
                    break;
                }
        }
    }
}

void CHandler::add(Point* v, float mass, bool fixed){
    _tangible.push_back({v, mass, fixed});
}

void CHandler::add(Physical t){
    _tangible.push_back(t);
}

void CHandler::collision(Physical& obj1, Physical& obj2) const {
    float m1 = obj2.fixed ? 0:obj1.mass;
    float m2 = obj1.fixed ? 0:obj2.mass;
    glm::vec3 impulse = 2.0f*(obj2.obj->vel - obj1.obj->vel)/(m2 + m1);
    glm::vec3 dirVec{0, 0, 0};
    if(!obj1.fixed)
        if(Polygon* obj2_surf = dynamic_cast<Polygon*>(obj2.obj))
            obj1.obj->vel += obj2_surf->normVec()*glm::dot(obj2_surf->normVec(), obj2.mass*impulse);
        else if(Polyhedron* obj2_sol = dynamic_cast<Polyhedron*>(obj2.obj)){
            glm::vec3 vec{0, 0, 0};
            for(std::shared_ptr<Polygon> p: obj2_sol->faces)
                if(obj1.obj->dist(*p) == 0)
                    vec += p->normVec();
            vec = glm::normalize(vec);
            std::cout << vec << "1" << std::endl;
            obj1.obj->vel += vec*glm::dot(vec, obj2.mass*impulse);
        }
    if(!obj2.fixed)
        if(Polygon* obj1_surf = dynamic_cast<Polygon*>(obj1.obj))
            obj2.obj->vel -= obj1_surf->normVec()*glm::dot(obj1_surf->normVec(), obj1.mass*impulse);
        else if(Polyhedron* obj1_sol = dynamic_cast<Polyhedron*>(obj1.obj)){
            glm::vec3 vec{0, 0, 0};
            for(std::shared_ptr<Polygon> p: obj1_sol->faces)
                if(obj2.obj->dist(*p) == 0)
                    vec += p->normVec();
            vec = glm::normalize(vec);
            std::cout << vec << "2" << std::endl;
            obj2.obj->vel -= vec*glm::dot(vec, obj1.mass*impulse);
            std::cout << obj2.obj->vel << std::endl;
        }
}

void CHandler::handle(Polygon* s1, Polygon* s2) const { // TODO: use these functions to find contact directions

}

void CHandler::handle(Polygon* s1, Polyhedron* s2) const {

}

void CHandler::handle(Polyhedron* s1, Polygon* s2) const {}

void CHandler::handle(Polyhedron* s1, Polyhedron* s2) const {}
