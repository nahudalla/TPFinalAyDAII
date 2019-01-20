#include "../Test.hpp"

#include <algor/Point.hpp>
#include <algor/Vector.hpp>

using algor::Vector;
using algor::Point;

bool vector_constructor(); ADD_TEST(VECTOR_CONSTRUCTOR, vector_constructor);
bool vector_cross_product(); ADD_TEST(VECTOR_CROSS_PRODUCT, vector_cross_product);
bool vector_from_points(); ADD_TEST(VECTOR_FROM_POINTS, vector_from_points);
bool vector_clockwise(); ADD_TEST(VECTOR_CLOCKWISE, vector_clockwise);
bool vector_counterclockwise(); ADD_TEST(VECTOR_COUNTERCLOCKWISE, vector_counterclockwise);
bool vector_colinear(); ADD_TEST(VECTOR_COLINEAR, vector_colinear);

bool vector_constructor() {
    for (int x = -10; x < 10; ++x) {
        for (int y = -10; y < 10; ++y) {
            Vector v(x, y);

            if(v.getX() != x || v.getY() != y) return false;
        }
    }

    return true;
}

bool vector_from_points() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Point p1(x1, y1);

            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Point p2(x2, y2);

                    Vector v1 = p1-p2;
                    if (v1.getX() != x1-x2
                        || v1.getY() != y1-y2) {
                        return false;
                    }

                    Vector v2 = p2-p1;
                    if (v2.getX() != x2-x1
                        || v2.getY() != y2-y1) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool vector_cross_product() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Vector v1(x1, y1);

            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Vector v2(x2, y2);

                    if(v1.crossProduct(v1) != 0
                        || v2.crossProduct(v2) != 0
                        || v1.crossProduct(v2) != (x1*y2-y1*x2)
                        || v2.crossProduct(v1) != (x2*y1-y2*x1)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool vector_clockwise() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Vector v1(x1, y1);

            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Vector v2(x2, y2);

                    if ((v1.isClockwise(v2) && v2.crossProduct(v1) <= 0)
                        || (v2.isClockwise(v1) && v1.crossProduct(v2) <= 0)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool vector_counterclockwise() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Vector v1(x1, y1);

            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Vector v2(x2, y2);

                    if ((v1.isCounterClockwise(v2) && v2.crossProduct(v1) >= 0)
                        || (v2.isCounterClockwise(v1) && v1.crossProduct(v2) >= 0)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool vector_colinear() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Vector v1(x1, y1);

            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Vector v2(x2, y2);

                    if ((v1.isColinear(v2) && v2.crossProduct(v1) != 0)
                        || (v2.isColinear(v1) && v1.crossProduct(v2) != 0)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}
