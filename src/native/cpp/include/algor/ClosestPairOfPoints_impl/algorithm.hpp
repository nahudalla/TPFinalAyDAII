#ifndef TPFINALAYDAI_ALGOR_CLOSEST_PAIR_OF_POINTS_ALGORITHM_HPP
#define TPFINALAYDAI_ALGOR_CLOSEST_PAIR_OF_POINTS_ALGORITHM_HPP

#include <utility>
#include <cstddef>
#include <cstdint>

#include <algor/Point.hpp>
#include <algor/Segment.hpp>

namespace algor::__detail_ClosestPairOfPoints {
    std::pair<Point, Point> findClosest(Point *Px, Point *Py, std::size_t n);
}

#endif //TPFINALAYDAI_ALGOR_CLOSEST_PAIR_OF_POINTS_ALGORITHM_HPP
