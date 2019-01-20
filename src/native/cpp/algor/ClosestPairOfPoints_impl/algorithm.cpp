#include <utility>
#include <cstddef>
#include <cstdint>
#include <optional>

#include <algor/Point.hpp>
#include <algor/Segment.hpp>

#include <algor/ClosestPairOfPoints_impl/algorithm.hpp>

namespace algor::__detail_ClosestPairOfPoints {
    std::pair<Point, Point> bruteForce(Point points[], std::size_t n) {
        std::size_t minSquaredDistance = SIZE_MAX;
        std::pair<Point, Point> closest_points;

        for(std::size_t i = 0; i < n; ++i) {
            for(std::size_t j = i+1; j < n; ++j) {
                auto sqDistance = Segment(points[i], points[j]).squaredLength();
                if(sqDistance < minSquaredDistance) {
                    minSquaredDistance = sqDistance;
                    closest_points = std::make_pair(points[i], points[j]);
                }
            }
        }

        return closest_points;
    }

    auto squaredYDistance(const Point & p1, const Point & p2) {
        return Segment(Point(0, p1.getY()), Point(0, p2.getY())).squaredLength();
    }

    auto squaredXDistance(const Point & p1, const Point & p2) {
        return Segment(Point(p1.getX(), 0), Point(p2.getX(), 0)).squaredLength();
    }

    auto stripClosest(Point strip[], std::size_t n, std::size_t squaredDistance) {
        std::size_t minSquaredDistance = squaredDistance;
        std::optional<std::pair<Point, Point>> closestPoints;

        for(std::size_t i = 0; i < n; ++i) {
            for(std::size_t j = i+1; j < n && squaredYDistance(strip[i], strip[j]) < minSquaredDistance; ++j) {
                auto sqDistance = Segment(strip[i], strip[j]).squaredLength();
                if(sqDistance < minSquaredDistance) {
                    minSquaredDistance = sqDistance;
                    closestPoints = std::make_pair(strip[i], strip[j]);
                }
            }
        }

        return closestPoints;
    }

    void sortPointsByY(Point points[], std::size_t n) {
        for(std::size_t i = 0; i < n; ++i) {
            for(std::size_t j = i+1; j < n; ++j) {
                if(points[j].getY() < points[i].getY()) {
                    std::swap(points[i], points[j]);
                }
            }
        }
    }

    void mergeY(Point dest[], Point a[], Point b[], std::size_t size_a, std::size_t size_b) {
        std::size_t i_a = 0, i_b = 0, i_dest = 0;

        while(i_a < size_a && i_b < size_b) {
            if(a[i_a].getY() < b[i_b].getY()) {
                dest[i_dest++] = a[i_a++];
            } else {
                dest[i_dest++] = b[i_b++];
            }
        }

        for(; i_a < size_a; ++i_a) {
            dest[i_dest++] = a[i_a];
        }

        for(; i_b < size_b; ++i_b) {
            dest[i_dest++] = b[i_b];
        }
    }

    void divideY(Point left[], Point right[], Point points[], std::size_t n) {
        std::size_t mid = n/2;
        std::size_t i = 0, i_l = 0, i_r = 0;
        while(i < n) {
            if(i_l < mid) {
                left[i_l++] = points[i++];
            } else {
                right[i_r++] = points[i++];
            }
        }
    }

    std::pair<Point, Point> findClosest(Point Px[], Point Py[], std::size_t n) {
        if (n <= 3) {
            sortPointsByY(Py, n);

            return bruteForce(Px, n);
        }

        std::pair<Point, Point> closestPair;
        std::size_t minSqDistance;
        Point midPoint;

        {
            // Find the middle point
            std::size_t mid = n / 2;

            midPoint = Px[mid];

            Point Pyl[mid], Pyr[n - mid];
            divideY(Pyl, Pyr, Py, n);

            // Consider the vertical line passing through the middle point
            // calculate the closest points pl on left of middle point and
            // pr on right side
            auto pl = findClosest(Px, Pyl, mid);
            auto pr = findClosest(Px + mid, Pyr, n - mid);

            mergeY(Py, Pyl, Pyr, mid, n-mid);

            // Find the closest of the two pairs
            auto sqDistanceLeft = Segment(pl.first, pl.second).squaredLength();
            auto sqDistanceRight = Segment(pr.first, pr.second).squaredLength();

            if (sqDistanceRight < sqDistanceLeft) {
                minSqDistance = sqDistanceRight;
                closestPair = pr;
            } else {
                minSqDistance = sqDistanceLeft;
                closestPair = pl;
            }
        }

        // Build an array strip[] that contains points close (closer than d)
        // to the line passing through the middle point
        Point strip[n];
        std::size_t j = 0;
        for(std::size_t i = 0; i < n; i++) {
            if(squaredXDistance(Py[i], midPoint) < minSqDistance) {
                strip[j] = Py[i];
                ++j;
            }
        }

        // Find the closest points in strip.  Return the minimum of d and closest
        // distance is strip[]
        auto stripClosestPair = stripClosest(strip, j, minSqDistance);
        return (!stripClosestPair.has_value()
                || minSqDistance < Segment(stripClosestPair->first, stripClosestPair->second).squaredLength())
                ? closestPair
                : *stripClosestPair;
    }
}
