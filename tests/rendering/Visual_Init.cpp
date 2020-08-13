#include <gtest/gtest.h>
#include "Graphics/window.hpp"

struct VisualInitTest: public ::testing::Test {
    gmh::Window* window;

    virtual void SetUp() override {
        window = new gmh::Window(480, 480, "Test Window");
    }

    virtual void TearDown() override {
        delete window;
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
