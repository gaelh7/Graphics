#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct PolyhedronDistTest: public ::testing::Test {
    Polyhedron poly;
    virtual void SetUp() override {
        poly = Polyhedron(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0));
    }
};

TEST_F(PolyhedronDistTest, PolyhedronDistToPolyhedron){
    Polyhedron poly2(glm::vec3(0, 0, 0.5), glm::vec3(-1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(1, 1, -1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(0, poly.dist(poly2));

    poly2 = Polyhedron(glm::vec3(0, 0, -0.5), glm::vec3(-1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(1, 1, -1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(0.5, poly.dist(poly2));

    poly2 = Polyhedron(glm::vec3(3, 0, 1), glm::vec3(2, 1, 0), glm::vec3(4, -1, 0), glm::vec3(2, -1, 0), glm::vec3(4, 1, 0));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(1, poly.dist(poly2));

    poly2 = Polyhedron(glm::vec3(3, 3, 1), glm::vec3(4, 4, 0), glm::vec3(4, 2, 0), glm::vec3(2, 2, 0), glm::vec3(2, 4, 0));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(sqrt(2), poly.dist(poly2));

    poly2 = Polyhedron(glm::vec3(2, 0, 2), glm::vec3(1, 1, 1), glm::vec3(3, -1, 1), glm::vec3(1, -1, 1), glm::vec3(3, 1, 1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(sqrt(0.5), poly.dist(poly2));

    poly2 = Polyhedron(glm::vec3(2, 2, 2), glm::vec3(3, 3, 1), glm::vec3(3, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 3, 1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(sqrt(2.0/3.0), poly.dist(poly2));

    poly2 = Polyhedron(glm::vec3(0, 0, 2), glm::vec3(-4, 4, -1), glm::vec3(4, -4, -1), glm::vec3(-4, -4, -1), glm::vec3(4, 4, -1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(0, poly.dist(poly2));
}
