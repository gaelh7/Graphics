#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct LinSegInterTest: public ::testing::Test {
    gmh::LinSeg obj;
    virtual void SetUp() override {
        obj = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0));
    }

    template<typename T>
    inline void checkNoInter(const T& obj2){
        EXPECT_FALSE(obj2.intersect(obj)) << "Intersection: " << *obj2.intersect(obj);
        EXPECT_FALSE(obj.intersect(obj2)) << "Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }

    template<typename T1, typename T2>
    inline void checkYesInter(const T1& obj2, const T2&& inter){
        ASSERT_TRUE(obj.intersect(obj2));
        ASSERT_TRUE(obj2.intersect(obj));
        EXPECT_TRUE(obj.intersect(obj2)->equals(*obj2.intersect(obj))) << "obj.intersect(obj2): " << *obj.intersect(obj2) << std::endl << "obj2.intersect(obj): " << *obj2.intersect(obj);
        EXPECT_TRUE(obj.intersect(obj2)->equals(inter)) << "Expected Intersection: " << inter << std::endl << "Actual Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }
};

TEST_F(LinSegInterTest, LinSegInterWithLinSeg){
    gmh::LinSeg lin2(glm::vec3(0.5, 1, 0), glm::vec3(1.5, 1, 0));
    checkNoInter(lin2);

    lin2 = gmh::LinSeg(glm::vec3(2, 1, 0), glm::vec3(3, 2, 0));
    checkNoInter(lin2);

    lin2 = gmh::LinSeg(glm::vec3(-1, 1, 0), glm::vec3(-1, -1, 0));
    checkNoInter(lin2);

    lin2 = gmh::LinSeg(glm::vec3(-1, 0, 0), glm::vec3(3, 0, 0));
    checkYesInter(lin2, gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0)));

    lin2 = gmh::LinSeg(glm::vec3(0, -1, 0), glm::vec3(2, 1, 0));
    checkYesInter(lin2, gmh::Point({1, 0, 0}));

    lin2 = gmh::LinSeg(glm::vec3(2, 1, 0), glm::vec3(2, -1, 0));
    checkYesInter(lin2, gmh::Point({2, 0, 0}));

    lin2 = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(3, 1, 0));
    checkYesInter(lin2, gmh::Point({0, 0, 0}));
}

TEST_F(LinSegInterTest, LinSegInterWithPlane){
    gmh::Plane plan(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(0, 1, 1));

    checkNoInter(plan);

    obj = gmh::LinSeg(glm::vec3(0, 0, -1), glm::vec3(0, 0, -2));
    checkNoInter(plan);

    obj = gmh::LinSeg(glm::vec3(0, 0, 2), glm::vec3(1, 1, 3));
    checkNoInter(plan);

    obj = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(0, 0, 2));
    checkYesInter(plan, gmh::Point({0, 0, 1}));

    obj = gmh::LinSeg(glm::vec3(186, -12, 1), glm::vec3(44, 99, 2));
    checkYesInter(plan, gmh::Point({186, -12, 1}));

    obj = gmh::LinSeg(glm::vec3(20, -40, 1), glm::vec3(-11, 9, 1));
    checkYesInter(plan, gmh::LinSeg(glm::vec3(20, -40, 1), glm::vec3(-11, 9, 1)));
}

TEST_F(LinSegInterTest, LinSegInterWithPolygon){
    gmh::Polygon poly(glm::vec3(-1, -1, 0), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(1, 1, 0));

    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));

    obj = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(2, 2, 0));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0)));

    obj = gmh::LinSeg(glm::vec3(2, 0, 0), glm::vec3(0, 2, 0));
    checkYesInter(poly, gmh::Point({1, 1, 0}));

    obj = gmh::LinSeg(glm::vec3(4, 0, 0), glm::vec3(0, 4, 0));
    checkNoInter(poly);

    obj = gmh::LinSeg(glm::vec3(2, 1, -1), glm::vec3(2, 1, 1));
    checkNoInter(poly);

    obj = gmh::LinSeg(glm::vec3(0, 0, 1), glm::vec3(2, 1, 1));
    checkNoInter(poly);

    obj = gmh::LinSeg(glm::vec3(0.5, 0.5, 5), glm::vec3(0.5, 0.5, -5));
    checkYesInter(poly, gmh::Point({0.5, 0.5, 0}));

    obj = gmh::LinSeg(glm::vec3(-0.5, -0.5, 0), glm::vec3(0.5, 0.5, 0));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(-0.5, -0.5, 0), glm::vec3(0.5, 0.5, 0)));
}

TEST_F(LinSegInterTest, LinSegInterWithPolyhedron){
    gmh::Polyhedron poly(glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0));

    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));

    obj = gmh::LinSeg(glm::vec3(1, 2, 0), glm::vec3(1, -2, 0));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0)));

    obj = gmh::LinSeg(glm::vec3(0, 0, 0.2), glm::vec3(0, 0, 0.8));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 0, 0.2), glm::vec3(0, 0, 0.8)));

    obj = gmh::LinSeg(glm::vec3(0, 0, 2), glm::vec3(0, 0, -1));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)));

    obj = gmh::LinSeg(glm::vec3(1, 1, 1), glm::vec3(-1, -1, 1));
    checkYesInter(poly, gmh::Point({0, 0, 1}));

    obj = gmh::LinSeg(glm::vec3(1, 0, 1), glm::vec3(1, 0, -1));
    checkYesInter(poly, gmh::Point({1, 0, 0}));

    obj = gmh::LinSeg(glm::vec3(0.5, 0, 1), glm::vec3(0.5, 0, -1));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0.5, 0, 0), glm::vec3(0.5, 0, 0.5)));

    obj = gmh::LinSeg(glm::vec3(0, 0, -1), glm::vec3(0, 0, -2));
    checkNoInter(poly);

    obj = gmh::LinSeg(glm::vec3(2, 1, 0), glm::vec3(2, -1, 0));
    checkNoInter(poly);

    obj = gmh::LinSeg(glm::vec3(2, 0, 0), glm::vec3(3, 0, 0));
    checkNoInter(poly);
}
