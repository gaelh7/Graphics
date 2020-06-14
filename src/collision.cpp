#include <glm/geometric.hpp>
#include "Graphics/collision.hpp"
#include "Graphics/gmath.hpp"

std::vector<std::vector<Physical>> CHandler::get_check() const {
    return combinations<Physical>(tangible.begin(), tangible.end(), 2);
}

CHandler::CHandler(){}

void CHandler::operator()() const {
    std::vector<std::vector<Physical>> objects = get_check();
    for(std::vector<Physical> v: objects){
        if(v[0].fixed && v[1].fixed) continue;
        // collision(v[0]s, v[1]);
        if(Polygon* v0_surf = dynamic_cast<Polygon*>(v[0].obj)){
            if(v[1]->dist(*v0_surf) == 0 && glm::dot(v[1]->vel - v0_surf->vel, v0_surf->normVec()*v0_surf->sign_dist(*v[1].obj)) < 0)
                collision(v[0], v[1]);
        }
        else if(Polyhedron* v0_sol = dynamic_cast<Polyhedron*>(v[0].obj)){
            for(std::shared_ptr<Polygon> face: v0_sol->faces)
                if(v[1]->dist(*v0_sol) == 0 && glm::dot(v[1]->vel - v0_sol->vel, face->normVec()*face->sign_dist(*v[1].obj)) < 0){
                    collision(v[0], v[1]);
                    break;
                }
        }
    }
}

void CHandler::add(Point* v, float mass, bool fixed){
    tangible.insert({v, mass, fixed});
}

void CHandler::add(Physical t){
    tangible.insert(t);
}

void CHandler::collision(Physical& obj1, Physical& obj2) const {
    float m1 = obj2.fixed ? 0:obj1.mass;
    float m2 = obj1.fixed ? 0:obj2.mass;
    glm::vec3 impulse = 2.0f*(obj2->vel - obj1->vel)/(m2 + m1);
    glm::vec3 dirVec1{0, 0, 0};
    glm::vec3 dirVec2{0, 0, 0};
    if(!obj1.fixed)
        if(Polygon* obj2_surf = dynamic_cast<Polygon*>(obj2.obj)){
            if(obj1->dist(*obj2_surf) == 0 && glm::dot(obj1->vel - obj2->vel, obj2_surf->normVec()*obj2_surf->sign_dist(*obj1.obj)) < 0)
                dirVec1 = obj2_surf->normVec();
        }
        else if(Polyhedron* obj2_sol = dynamic_cast<Polyhedron*>(obj2.obj)){
            for(std::shared_ptr<Polygon> p: obj2_sol->faces)
                if(obj1->dist(*p) == 0 && glm::dot(obj1->vel - obj2->vel, p->normVec()*p->sign_dist(*obj1.obj)) < 0){
                    dirVec1 += p->normVec();
                }
            // vec = glm::normalize(vec);
            std::cout << dirVec1 << "1" << std::endl;
        }
    if(!obj2.fixed)
        if(Polygon* obj1_surf = dynamic_cast<Polygon*>(obj1.obj)){
            if(obj2->dist(*obj1_surf) == 0 && glm::dot(obj2->vel - obj1->vel, obj1_surf->normVec()*obj1_surf->sign_dist(*obj2.obj)) < 0)
                dirVec2 = obj1_surf->normVec();
        }
        else if(Polyhedron* obj1_sol = dynamic_cast<Polyhedron*>(obj1.obj)){
            for(std::shared_ptr<Polygon> p: obj1_sol->faces)
                if(obj2->dist(*p) == 0 && glm::dot(obj2->vel - obj1->vel, p->normVec()*p->sign_dist(*obj2.obj)) < 0){
                    dirVec2 += p->normVec();
                }
            // vec = glm::normalize(vec);
            std::cout << dirVec2 << "2" << std::endl;
            std::cout << obj2->vel << std::endl;
        }
    obj1->vel += dirVec1*glm::dot(dirVec1, obj2.mass*impulse);
    obj2->vel -= dirVec2*glm::dot(dirVec2, obj2.mass*impulse);
}

void CHandler::handle(Polygon* s1, Polygon* s2) const { // TODO: use these functions to find contact directions

}

void CHandler::handle(Polygon* s1, Polyhedron* s2) const {

}

void CHandler::handle(Polyhedron* s1, Polygon* s2) const {}

void CHandler::handle(Polyhedron* s1, Polyhedron* s2) const {}
