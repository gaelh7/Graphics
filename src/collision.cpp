#include "Graphics/collision.hpp"
#include "Graphics/gmath.hpp"

std::vector<std::vector<Tangible>> CHandler::get_check(){
    return combinations<Tangible>(_tangible.begin(), _tangible.end(), 2);
}

void CHandler::operator()(){
    std::vector<std::vector<Tangible>> objects = get_check();
    for(std::vector<Tangible> v: objects){
        if(Surface* v0_surf = dynamic_cast<Surface*>(v[0].obj)){
            if(Surface* v1_surf = dynamic_cast<Surface*>(v[1].obj))
                collision(v0_surf, v1_surf);
            else if(Solid* v1_sol = dynamic_cast<Solid*>(v[1].obj))
                collision(v0_surf, v1_sol);
        }
        if(Solid* v0_sol = dynamic_cast<Solid*>(v[0].obj)){
            if(Surface* v1_surf = dynamic_cast<Surface*>(v[1].obj))
                collision(v1_surf, v0_sol);
            else if(Solid* v1_sol = dynamic_cast<Solid*>(v[1].obj))
                collision(v0_sol, v1_sol);
        }
    }
}

void CHandler::add(Visual* v, bool fixed){
    _tangible.push_back({v, fixed});
}

void CHandler::add(Tangible t){
    _tangible.push_back(t);
}

void CHandler::collision(Surface* s1, Surface* s2){}

void CHandler::collision(Surface* s1, Solid* s2){
    if(s2->dist(*s1) == 0){
        if(glm::dot(s2->vel, s2->pos - s1->vertices[0]->pos) < 0)
            s2->vel -= glm::dot(s2->vel, s1->normVec()) * s1->normVec();
    }
}

void CHandler::collision(Solid* s1, Surface* s2){}

void CHandler::collision(Solid* s1, Solid* s2){}
