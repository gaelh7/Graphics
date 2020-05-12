#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

enum Direction {
    FORWARD, LEFT, BACKWARD, RIGHT
};

class Camera {
    private:
        glm::vec3 worldUp;
    public:
        void update();
        glm::vec3 pos, front, right, up;
        float yaw, pitch, speed, sensitivity, zoom;
        Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);
        glm::mat4 view();
        void key_press(Direction d, float dt);
        void mouse_move(float dx, float dy, bool constrain = true);
        void mouse_scroll(float offset);
};
