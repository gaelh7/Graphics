#include <gtest/gtest.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct VisualInitTest: public ::testing::Test {
    unsigned int VAO;

    virtual void SetUp() override {
        GLFWwindow* window;

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        int width = 480;
        int height = 480;
        window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    }
};

TEST_F(VisualInitTest, FailCases){
    // EXPECT_THROW(gmh::Surface(glm::vec3(1, 0, 0), glm::vec3(2, 0, 0), glm::vec3(3, 0, 0)), std::invalid_argument);
    // EXPECT_THROW(gmh::Surface(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 0)), std::invalid_argument);
    // EXPECT_THROW(gmh::Surface(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0.01)), std::invalid_argument);
    // EXPECT_THROW(gmh::Surface(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0, 0)), std::invalid_argument);
    // EXPECT_THROW(gmh::Surface(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 1, 0)), std::invalid_argument);

    // EXPECT_THROW(gmh::Solid(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)), std::invalid_argument);
    // EXPECT_THROW(gmh::Solid(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(0.5, 0.5, 0.5)), std::invalid_argument);
    // EXPECT_THROW(gmh::Solid(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(0.5, 0.5, 0)), std::invalid_argument);
    // EXPECT_THROW(gmh::Solid(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(1, 0, 0)), std::invalid_argument);
    // EXPECT_THROW(gmh::Solid(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(1, 1, 0)), std::invalid_argument);
}
