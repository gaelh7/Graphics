#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct PolygonInterTest: public ::testing::Test {
    Polygon obj;

    virtual void SetUp() override {
        obj = Polygon(glm::vec3(2, 2, 0), glm::vec3(-2, 2, 0), glm::vec3(2, -2, 0), glm::vec3(-2, -2, 0));
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

TEST_F(PolygonInterTest, PolygonInterWithPolygon){
    Polygon poly2(glm::vec3(0, 0, 0), glm::vec3(1, 0, 1), glm::vec3(-1, 0, 1));
    checkYesInter(poly2, Point({0, 0, 0}));

    poly2 = Polygon(glm::vec3(0, 1, 1), glm::vec3(1, 0, 1), glm::vec3(-1, 0, 1));
    checkNoInter(poly2);

    poly2 = Polygon(glm::vec3(3, 1, 0), glm::vec3(3, -1, 0), glm::vec3(4, 1, 0), glm::vec3(4, -1, 0));
    checkNoInter(poly2);

    poly2 = Polygon(glm::vec3(3, 1, 1), glm::vec3(3, -1, -1), glm::vec3(3, 1, -1), glm::vec3(3, -1, 1));
    checkNoInter(poly2);

    poly2 = Polygon(glm::vec3(0, 0, 1), glm::vec3(0, 5, 2), glm::vec3(0, -5, 2));
    checkNoInter(poly2);

    poly2 = Polygon(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, 1, 0), glm::vec3(-1, -1, 0));
    checkYesInter(poly2, Polygon(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, 1, 0), glm::vec3(-1, -1, 0)));

    poly2 = Polygon(glm::vec3(0, 0, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0));
    checkYesInter(poly2, Polygon(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), glm::vec3(2, 2, 0)));

    poly2 = Polygon(glm::vec3(0, 0, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), glm::vec3(4, 4, 0));
    checkYesInter(poly2, Polygon(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), glm::vec3(2, 2, 0)));

    poly2 = Polygon(glm::vec3(0, 3, 0), glm::vec3(0, -3, 0), glm::vec3(3, 3, 0), glm::vec3(3, -3, 0));
    checkYesInter(poly2, Polygon(glm::vec3(0, 2, 0), glm::vec3(0, -2, 0), glm::vec3(2, -2, 0), glm::vec3(2, 2, 0)));

    poly2 = Polygon(glm::vec3(2, 3, 0), glm::vec3(2, -3, 0), glm::vec3(3, 3, 0), glm::vec3(3, -3, 0));
    checkYesInter(poly2, LinSeg(glm::vec3(2, 2, 0), glm::vec3(2, -2, 0)));

    poly2 = Polygon(glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), glm::vec3(4, 4, 0));
    checkYesInter(poly2, Point({2, 2, 0}));

    poly2 = Polygon(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1));
    checkYesInter(poly2, LinSeg(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)));

    poly2 = Polygon(glm::vec3(0, 3, 0), glm::vec3(0, -3, 0), glm::vec3(0, 0, 1));
    checkYesInter(poly2, LinSeg(glm::vec3(0, 2, 0), glm::vec3(0, -2, 0)));

    poly2 = Polygon(glm::vec3(0, 4, 0), glm::vec3(4, 0, 0), glm::vec3(3, 1, 2));
    checkYesInter(poly2, Point({2, 2, 0}));

    poly2 = Polygon(glm::vec3(2, 2, 0), glm::vec3(2, -2, 0), glm::vec3(2, 0, 2));
    checkYesInter(poly2, LinSeg(glm::vec3(2, 2, 0), glm::vec3(2, -2, 0)));
}

TEST_F(PolygonInterTest, PolygonInterWithPolyhedron){
    Polyhedron polyhed(glm::vec3(2, 2, 0), glm::vec3(-2, 2, 0), glm::vec3(2, -2, 0), glm::vec3(-2, -2, 0), glm::vec3(0, 0, 2));
    checkYesInter(polyhed, Polygon(glm::vec3(2, 2, 0), glm::vec3(-2, 2, 0), glm::vec3(2, -2, 0), glm::vec3(-2, -2, 0)));

    obj = Polygon(glm::vec3(2, 2, 1), glm::vec3(-2, 2, 1), glm::vec3(2, -2, 1), glm::vec3(-2, -2, 1));
    checkYesInter(polyhed, Polygon(glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1)));

    obj = Polygon(glm::vec3(2, 2, 2), glm::vec3(-2, 2, 2), glm::vec3(2, -2, 2), glm::vec3(-2, -2, 2));
    checkYesInter(polyhed, Point({0, 0, 2}));

    obj = Polygon(glm::vec3(6, 2, 0), glm::vec3(2, 2, 0), glm::vec3(6, -2, 0), glm::vec3(2, -2, 0));
    checkYesInter(polyhed, LinSeg(glm::vec3(2, 2, 0), glm::vec3(2, -2, 0)));

    obj = Polygon(glm::vec3(2, 2, 2), glm::vec3(2, 2, -2), glm::vec3(2, -2, 2), glm::vec3(2, -2, -2));
    checkYesInter(polyhed, LinSeg(glm::vec3(2, 2, 0), glm::vec3(2, -2, 0)));

    obj = Polygon(glm::vec3(5, 2, 1), glm::vec3(1, 2, 1), glm::vec3(5, -2, 1), glm::vec3(1, -2, 1));
    checkYesInter(polyhed, LinSeg(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1)));

    obj = Polygon(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(-1, 1, 0));
    checkYesInter(polyhed, Polygon(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(-1, 1, 0)));

    obj = Polygon(glm::vec3(1, 1, 0.5), glm::vec3(1, -1, 0.5), glm::vec3(-1, -1, 0.5), glm::vec3(-1, 1, 0.5));
    checkYesInter(polyhed, Polygon(glm::vec3(1, 1, 0.5), glm::vec3(1, -1, 0.5), glm::vec3(-1, -1, 0.5), glm::vec3(-1, 1, 0.5)));

    obj = Polygon(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, 1));
    checkYesInter(polyhed, Polygon(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, 1)));

    obj = Polygon(glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(-1, 1, -1));
    checkNoInter(polyhed);

    obj = Polygon(glm::vec3(5, 1, 0), glm::vec3(5, -1, 0), glm::vec3(3, -1, 0), glm::vec3(3, 1, 0));
    checkNoInter(polyhed);

    obj = Polygon(glm::vec3(0, 1, -1), glm::vec3(0, -1, -1), glm::vec3(0, -1, -2), glm::vec3(0, 1, -2));
    checkNoInter(polyhed);
}
