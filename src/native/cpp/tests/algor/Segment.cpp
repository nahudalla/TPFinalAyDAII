#include "../Test.hpp"

#include <algor/Point.hpp>
#include <algor/Segment.hpp>

using algor::Point;
using algor::Segment;

bool segment_getters(); ADD_TEST(SEGMENT_GETTERS, segment_getters);
bool segment_contains(); ADD_TEST(SEGMENT_CONTAINS, segment_contains);
bool segment_intersects(); ADD_TEST(SEGMENT_INTERSECTS, segment_intersects);
bool segment_turns_left(); ADD_TEST(SEGMENT_TURNS_LEFT, segment_turns_left);
bool segment_turns_right(); ADD_TEST(SEGMENT_TURNS_RIGHT, segment_turns_right);
bool segment_is_colinear(); ADD_TEST(SEGMENT_IS_COLINEAR, segment_is_colinear);
bool segment_is_clockwise(); ADD_TEST(SEGMENT_IS_CLOCKWISE, segment_is_clockwise);
bool segment_is_counter_clockwise(); ADD_TEST(SEGMENT_IS_COUNTER_CLOCKWISE, segment_is_counter_clockwise);

bool segment_getters() {
    for (int x1 = -10; x1 < 10; ++x1) {
        for (int y1 = -10; y1 < 10; ++y1) {
            Point p1(x1, y1);

            for (int x2 = -10; x2 < 10; ++x2) {
                for (int y2 = -10; y2 < 10; ++y2) {
                    Point p2(x2, y2);

                    Segment s1(p1, p2);
                    if(s1.getFrom() != p1 || s1.getTo() != p2) return false;

                    Segment s2(p2, p1);
                    if(s2.getFrom() != p2 || s2.getTo() != p1) return false;
                }
            }
        }
    }

    return true;
}

bool segment_contains() {
    Segment s1(Point(-10, 5), Point(10, 5)),
            s2(Point(5, -10), Point(5, 10)),
            s3(Point(-10, -10), Point(10, 10));

    for (int x = s1.getFrom().getX(); x <= s1.getTo().getX(); ++x) {
        if(!s1.contains(Point(x, 5))) return false;
    }

    for (int y = s2.getFrom().getY(); y <= s2.getTo().getY(); ++y) {
        if(!s2.contains(Point(5, y))) return false;
    }

    for (int x = s3.getFrom().getX(), y = s3.getFrom().getY();
            x <= s3.getTo().getX() && y <= s3.getTo().getY();
            ++x, ++y)
    {
        if(!s3.contains(Point(x, y))) return false;
    }

    return true;
}

bool segment_intersects() {
    {
        Segment s1(Point(-7, -13), Point(13, 7)), s2(Point(-8, -12), Point(14, 6));

        if (!s1.intersects(s2) || !s2.intersects(s1)) return false;
    }

    {
        Segment s1(Point(-10, -10), Point(10, 10)), s2(Point(5, 5), Point(10, -10));

        if (!s1.intersects(s2) || !s2.intersects(s1)) return false;
    }

    {
        Segment s1(Point(-7, -13), Point(13, 7)), s2(Point(-8, 5), Point(0, 0));

        if (s1.intersects(s2) || s2.intersects(s1)) return false;
    }

    {
        Segment s1(Point(-7, -13), Point(13, 7)), s2(Point(-8, -14), Point(14, 6));

        if (s1.intersects(s2) || s2.intersects(s1)) return false;
    }

    return true;
}

bool segment_turns_left() {
    Segment s1(Point(-8, -5), Point(6, 7)),
            s2(Point(6, 7), Point(-3, 9)),
            s3(Point(6, 7), Point(7, 4));

    return s1.turnsLeft(s2) && !s1.turnsLeft(s3);
}

bool segment_turns_right() {
    Segment s1(Point(-8, -5), Point(6, 7)),
            s2(Point(6, 7), Point(-3, 9)),
            s3(Point(6, 7), Point(7, 4));

    return s1.turnsRight(s3) && !s1.turnsRight(s2);
}

bool segment_is_colinear() {
    Segment s1(Point(-10, -10), Point(-3, -3)),
            s2(Point(-10, -10), Point(5, 5)),
            s3(Point(-10, -10), Point(5, -5));

    return s1.isColinear(s2) && s2.isColinear(s1) &&
            !s1.isColinear(s3) && !s3.isColinear(s1);
}

bool segment_is_clockwise() {
    Segment s1(Point(-10, -10), Point(-3, -3)),
            s2(Point(-10, -10), Point(5, -5)),
            s3(Point(-10, -10), Point(-5, 5));

    return s1.isClockwise(s2) && !s2.isClockwise(s1) &&
            !s1.isClockwise(s3) && s3.isClockwise(s1);
}

bool segment_is_counter_clockwise() {
    Segment s1(Point(-10, -10), Point(-3, -3)),
            s2(Point(-10, -10), Point(5, -5)),
            s3(Point(-10, -10), Point(-5, 5));

    return !s1.isCounterClockwise(s2) && s2.isCounterClockwise(s1) &&
           s1.isCounterClockwise(s3) && !s3.isCounterClockwise(s1);
}
