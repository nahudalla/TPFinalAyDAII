#include "../Test.hpp"

#include <algor/Point.hpp>

using algor::Point;

bool point_getters(); ADD_TEST(POINT_GETTERS, point_getters);
bool point_equality(); ADD_TEST(POINT_EQUALITY, point_equality);

bool point_getters() {
    for (int x = -10; x < 10; ++x) {
        for (int y = -10; y < 10; ++y) {
            Point p (x, y);

            if(p.getX() != x || p.getY() != y) return false;
        }
    }

    return true;
}

bool point_equality() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Point p1(x1, y1);
            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Point p2(x2, y2);

                    if(x1 == x2 && y1 == y2 && p1 != p2) return false;
                }
            }
        }
    }

    return true;
}
