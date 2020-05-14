#include "Graphics/camera.hpp"

inline void Camera::update(){
    front = glm::normalize(glm::vec3{cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch)});
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch): worldUp(up), pos(pos), front({0.f, 0.f, 1.f}), right(glm::normalize(glm::cross(front, worldUp))), up(glm::normalize(glm::cross(right, front))){
    this->yaw = yaw;
    this->pitch = pitch;
    speed = 2.5f;
    sensitivity = 0.01f;
    zoom = glm::quarter_pi<float>();
    update();
}

glm::mat4 Camera::view() {
    return glm::lookAt(pos, pos + front, up);
}

void Camera::key_press(Direction d, float dt) {
    float dx = speed*dt;
    switch (d){
        case FORWARD:
            pos += front*dx;
            break;
        case LEFT:
            pos -= right*dx;
            break;
        case BACKWARD:
            pos -= front*dx;
            break;
        case RIGHT:
            pos += right*dx;
            break;
    }
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
    update();
}

void Camera::mouse_scroll(float offset){
    zoom += offset*sensitivity;
    if(zoom <= glm::radians(1.f))
        zoom = glm::radians(1.f);
    else if(zoom >= glm::quarter_pi<float>())
        zoom = glm::quarter_pi<float>();
}
