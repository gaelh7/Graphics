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
        // collision(v[0], v[1]);
        if(Polygon* v0_surf = dynamic_cast<Polygon*>(v[0].obj)){
            if(v[1]->dist(*v0_surf) == 0)
                collision(v[0], v[1]);
        }
        else if(Polyhedron* v0_sol = dynamic_cast<Polyhedron*>(v[0].obj)){
            if(v[1]->dist(*v0_sol) == 0)
                collision(v[0], v[1]);
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
    glm::vec3 dirVec{0, 0, 0};
    int c1 = 0;
    int c2 = 0;
    for(std::shared_ptr<Point> p: obj2->vertices)
        if(obj1->contains(*p)) c1++;
    for(std::shared_ptr<Point> p: obj1->vertices)
        if(obj2->contains(*p)) c2++;
    if(c1 < c2){
        if(Polygon* obj2_surf = dynamic_cast<Polygon*>(obj2.obj)){
            if(glm::dot(obj1->vel - obj2->vel, obj2_surf->normVec()*obj2_surf->sign_dist(*obj1.obj)) < 0)
                dirVec = obj2_surf->normVec();
        }
        else if(Polyhedron* obj2_sol = dynamic_cast<Polyhedron*>(obj2.obj)){
            auto it = std::min_element(obj2_sol->faces.begin(), obj2_sol->faces.end(), [&obj1, &obj2](std::shared_ptr<Polygon> p1, std::shared_ptr<Polygon> p2){
                return obj1->dist(*p1) == 0 && glm::dot(obj1->vel - obj2->vel, p1->normVec()*(float)sign(p1->sign_dist(*obj1.obj))) < glm::dot(obj1->vel - obj2->vel, p2->normVec()*(float)sign(p2->sign_dist(*obj1.obj)));
            });
            dirVec = (*it)->normVec();
        }
    }
    else{
        if(Polygon* obj1_surf = dynamic_cast<Polygon*>(obj1.obj)){
            if(glm::dot(obj2->vel - obj1->vel, obj1_surf->normVec()*(float)sign(obj1_surf->sign_dist(*obj2.obj))) < 0)
                dirVec = obj1_surf->normVec();
        }
        else if(Polyhedron* obj1_sol = dynamic_cast<Polyhedron*>(obj1.obj)){
            auto it = std::min_element(obj1_sol->faces.begin(), obj1_sol->faces.end(), [&obj1, &obj2](std::shared_ptr<Polygon> p1, std::shared_ptr<Polygon> p2){
                return obj2->dist(*p1) == 0 && glm::dot(obj2->vel - obj1->vel, p1->normVec()*(float)sign(p1->sign_dist(*obj2.obj))) < glm::dot(obj2->vel - obj1->vel, p2->normVec()*(float)sign(p2->sign_dist(*obj2.obj)));
            });
            dirVec = (*it)->normVec();
        }
    }
    if(!obj1.fixed) obj1->vel += dirVec*glm::dot(dirVec, obj2.mass*impulse);
    if(!obj2.fixed) obj2->vel -= dirVec*glm::dot(dirVec, obj1.mass*impulse);
}

void CHandler::handle(Polygon* s1, Polygon* s2) const { // TODO: use these functions to find contact directions

}

void CHandler::handle(Polygon* s1, Polyhedron* s2) const {

}

void CHandler::handle(Polyhedron* s1, Polygon* s2) const {}

void CHandler::handle(Polyhedron* s1, Polyhedron* s2) const {}
