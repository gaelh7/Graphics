#include "Graphics/camera.hpp"

void Camera::update(){
    front = glm::normalize(glm::vec3{cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch)});
    right = glm::normalize(glm::cross(worldUp, front));
    up = glm::normalize(glm::cross(front, right));
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch): worldUp(up), pos(pos), front({0.f, 0.f, 1.f}), right(glm::normalize(glm::cross(front, worldUp))), up(glm::normalize(glm::cross(right, front))){
    this->yaw = yaw;
    this->pitch = pitch;
    speed = 2.5f;
    sensitivity = 0.01f;
    zoom = 45.f;
    update();
}

glm::mat4 Camera::view() {
    return glm::lookAt(pos, pos + front, worldUp);
}

void Camera::key_press(Direction d, float dt) {
    float dx = speed*dt;
    switch (d){
        case FORWARD:
            pos += front*dx;
            break;
        case LEFT:
            pos += right*dx;
            break;
        case BACKWARD:
            pos -= front*dx;
            break;
        case RIGHT:
            pos -= right*dx;
            break;
    }
}

void Camera::mouse_move(float dx, float dy, bool constrain){
    yaw += dx*sensitivity;
    pitch += dy*sensitivity;
    if (constrain){
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    update();
}

void Camera::mouse_scroll(float offset){
    zoom += offset;
    if(zoom <= 1.0f)
        zoom = 1.0f;
    else if(zoom >= 45.f)
        zoom = 45.f;
}
