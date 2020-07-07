#include "Graphics/camera.hpp"

inline void Camera::reset(){
    front = glm::normalize(glm::vec3{cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch)});
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch): worldUp(up), pos(pos), front(0, 0, 0), right(0, 0, 0), up(0, 0, 0), dir(NONE){
    this->yaw = yaw;
    this->pitch = pitch;
    speed = 2.5f;
    sensitivity = 0.01f;
    zoom = glm::quarter_pi<float>();
    reset();
}

glm::mat4 Camera::view(){
    return glm::lookAt(pos, pos + front, up);
}

void Camera::update(float dt){
    switch (dir){
        case FORWARD:
            pos += front*speed*dt;
            break;
        case LEFT:
            pos -= right*speed*dt;
            break;
        case BACKWARD:
            pos -= front*speed*dt;
            break;
        case RIGHT:
            pos += right*speed*dt;
            break;
    }
}

void Camera::set_dir(Direction d){
    dir = d;
}

void Camera::mouse_move(float dx, float dy, bool constrain){
    yaw += dx*sensitivity;
    pitch += dy*sensitivity;
    if(constrain){
        if (pitch > glm::half_pi<float>() - 0.01f)
            pitch = glm::half_pi<float>() - 0.01f;
        else if (pitch < 0.01f - glm::half_pi<float>())
            pitch = 0.01f - glm::half_pi<float>();
    }
    reset();
}

void Camera::mouse_scroll(float offset){
    zoom += offset*sensitivity;
    if(zoom <= glm::radians(1.f))
        zoom = glm::radians(1.f);
    else if(zoom >= glm::quarter_pi<float>())
        zoom = glm::quarter_pi<float>();
}
