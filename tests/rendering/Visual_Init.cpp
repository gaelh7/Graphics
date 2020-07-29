#include <gtest/gtest.h>
#include "Graphics/render.hpp"

struct VisualInitTest: public ::testing::Test {
    unsigned int VAO;

    virtual void SetUp() override {
        glGenVertexArrays(1, &VAO);
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
