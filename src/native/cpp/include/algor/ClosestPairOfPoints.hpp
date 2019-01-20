#ifndef TPFINALAYDAI_CLOSESTPAIROFPOINTS_HPP
#define TPFINALAYDAI_CLOSESTPAIROFPOINTS_HPP

#include <algor/Array.hpp>
#include <algor/Point.hpp>
#include <algor/Comparator.hpp>

#include <algor/ClosestPairOfPoints_impl/algorithm.hpp>

#include <optional>
#include <utility>

namespace algor {
    class ClosestPairOfPoints {
        typedef Comparator<Point>::ComparatorFunction CmpFn;
        typedef Comparator<Point>::Result CmpRes;

        std::optional<std::pair<Point, Point>> result;
        std::optional<Array<Point>> points_x;
        std::optional<Array<Point>> points_y;

    public:
        bool hasResult() const { return this->result.has_value(); }
        bool canRun() const {
            return this->points_x.has_value() && this->points_y.has_value();
        }

        explicit ClosestPairOfPoints(Array<Point> points) {
            if(points.size() < 2) return;

            this->points_x = points;
            this->points_y = std::move(points);

            this->points_x->sort((CmpFn) [](const Point & lhs, const Point & rhs) -> CmpRes {
                return lhs.getX() < rhs.getX()
                       ? CmpRes::LESS
                       : (rhs.getX() < lhs.getX() ? CmpRes::GREATER : CmpRes::EQUAL);
            });
        }

        auto run() const { return this->result; }

        std::optional<std::pair<Point, Point>> run() {
            if(this->hasResult()) return this->result;

            if(!this->canRun()) return std::nullopt;

            this->result = __detail_ClosestPairOfPoints::findClosest(
                    this->points_x->raw(),
                    this->points_y->raw(),
                    this->points_x->size()
            );

            this->points_x = std::nullopt;
            this->points_y = std::nullopt;

            return this->result;
        }
    };
}

#endif //TPFINALAYDAI_CLOSESTPAIROFPOINTS_HPP
