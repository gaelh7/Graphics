#include <glm/geometric.hpp>
#include "Graphics/collision.hpp"
#include "Graphics/gmath.hpp"
#include "Graphics/geometry.hpp"

using namespace gmh;

std::vector<std::vector<Physical>> CHandler::get_check() const {
    return combinations<Physical>(tangible.begin(), tangible.end(), 2);
}

CHandler::CHandler(): elasticity(2) {}

CHandler::CHandler(float elasticity): elasticity(elasticity + 1.f) {
    if(elasticity < 0 || elasticity > 1) throw std::domain_error("Elasticity must be a value between 0 and 1");
}

void CHandler::operator()() const {
    std::vector<std::vector<Physical>> objects = get_check();
    for(std::vector<Physical> v: objects){
        if(v[0].fixed && v[1].fixed) continue;
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

void CHandler::remove(Point* v){
    tangible.erase({v, 0, 0});
}

void CHandler::remove(Physical t){
    tangible.erase(t);
}

void CHandler::collision(Physical& obj1, Physical& obj2) const {
    float m1 = obj2.fixed ? 0:obj1.mass;
    float m2 = obj1.fixed ? 0:obj2.mass;
    glm::vec3 impulse = elasticity*(obj2->vel - obj1->vel)/(m2 + m1);
    glm::vec3 dirVec{0, 0, 0};
    if(Polygon* obj1_surf = dynamic_cast<Polygon*>(obj1.obj)){
        if(glm::dot(obj2->vel - obj1->vel, obj1_surf->normVec()*obj1_surf->sign_dist(*obj2.obj)) < 0)
            dirVec = obj1_surf->normVec();
    }
    else if(Polygon* obj2_surf = dynamic_cast<Polygon*>(obj2.obj)){
        if(glm::dot(obj1->vel - obj2->vel, obj2_surf->normVec()*obj2_surf->sign_dist(*obj1.obj)) < 0)
            dirVec = obj2_surf->normVec();
    }
    else if(Polyhedron* obj1_sol = dynamic_cast<Polyhedron*>(obj1.obj)) if(Polyhedron* obj2_sol = dynamic_cast<Polyhedron*>(obj2.obj)){
        float c1 = 0;
        float c2 = 0;
        for(std::shared_ptr<LinSeg> edge: obj1_sol->edges){
            if(std::unique_ptr<Point> inter = obj2_sol->intersect(*edge); LinSeg* line = dynamic_cast<LinSeg*>(inter.get())) c1 += line->length();
        }
        for(std::shared_ptr<LinSeg> edge: obj2_sol->edges){
            if(std::unique_ptr<Point> inter = obj1_sol->intersect(*edge); LinSeg* line = dynamic_cast<LinSeg*>(inter.get())) c2 += line->length();
        }
        if(c1 < c2){
            float val = 0;
            for(std::shared_ptr<Polygon> face: obj1_sol->faces){
                if(obj2->dist(*face) != 0) continue;
                float check = glm::dot(obj2->vel - obj1->vel, face->normVec()*static_cast<float>(sign(face->sign_dist(*obj2.obj))));
                if(check < val){
                    val = check;
                    dirVec = face->normVec();
                }
            }
        }
        else{
            float val = 0;
            for(std::shared_ptr<Polygon> face: obj2_sol->faces){
                if(obj1->dist(*face) != 0) continue;
                float check = glm::dot(obj1->vel - obj2->vel, face->normVec()*static_cast<float>(sign(face->sign_dist(*obj1.obj))));
                if(check < val){
                    val = check;
                    dirVec = face->normVec();
                }
            }
        }
    }
    obj1->vel += dirVec*glm::dot(dirVec, m2*impulse);
    obj2->vel -= dirVec*glm::dot(dirVec, m1*impulse);
}
