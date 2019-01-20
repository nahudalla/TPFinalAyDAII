#ifndef TPFINALAYDAI_ALGOR_GRAHAMSCAN_HPP
#define TPFINALAYDAI_ALGOR_GRAHAMSCAN_HPP

#include <optional>

#include <algor/Point.hpp>
#include <algor/List.hpp>
#include <algor/Stack.hpp>
#include <algor/Comparator.hpp>
#include <algor/Vector.hpp>
#include <algor/Segment.hpp>

namespace algor {
    class GrahamScan {
        std::optional<Point> p0;
        std::optional<List<Point>> sortedPoints;

        std::optional<Stack<Point>>  result;

    public:
        bool hasResult() const {
            return this->result.has_value();
        }

        bool canRun() const {
            return this->p0.has_value() && this->sortedPoints.has_value();
        }

        explicit GrahamScan(List<Point> points) {
            {
                // PRECONDICION: La lista tiene que tener al menos tres puntos que no sean colineales
                auto it1 = points.begin();
                auto it2 = it1; ++it2;
                auto it3 = it2; ++it3;
                auto end = points.end();

                if(it1 == end || it2 == end || it3 == end
                    || Vector(*it1, *it2).isColinear(Vector(*it1, *it3))) {
                    return;
                }
            }

            typedef Comparator<Point>::ComparatorFunction CompFn;
            typedef Comparator<Point>::Result CompRes;

            {
                auto it = points.findMinimum((CompFn) [](const Point &lhs, const Point &rhs) -> CompRes {
                    if (lhs.getY() < rhs.getY()) return CompRes::LESS;
                    else if (rhs.getY() < lhs.getY()) return CompRes::GREATER;
                    else if (lhs.getX() < rhs.getX()) return CompRes::LESS;
                    else if (rhs.getX() < lhs.getX()) return CompRes::GREATER;
                    else return CompRes::EQUAL;
                });

                this->p0 = points.remove(it);
            }
            const auto & p0 = *(this->p0);

            points.sort((CompFn) [&p0](const Point & lhs, const Point & rhs) -> CompRes {
                if(lhs == rhs) return CompRes::EQUAL;

                Vector v1(p0, lhs),
                       v2(p0, rhs);

                if(v1.isCounterClockwise(v2)) return CompRes::LESS;
                else if(v1.isClockwise(v2)) return CompRes::GREATER;
                else if(Segment(p0, lhs).contains(rhs)) return CompRes::GREATER;
                else return CompRes::LESS;
            });

            points.removeIf([&p0](List<Point>::Iterator const& _it, List<Point>::Iterator const& end)->bool {
                auto it = _it;
                auto next = it; ++next;
                if(next == end) return false;

                return Vector(p0, *it).isColinear(Vector(p0, *next));
            });

            this->sortedPoints = points;
        }

        std::optional<Stack<Point>> run() {
            if(this->result.has_value()) {
                return *(this->result);
            }

            if(!this->p0.has_value() || !this->sortedPoints.has_value()) {
                return std::nullopt;
            }

            auto it = this->sortedPoints->cbegin();
            auto end = this->sortedPoints->cend();

            Stack<Point> stack;

            stack.push(*(this->p0));
            stack.push(*(it++));
            stack.push(*(it++));

            for(; it != end; ++it) {
                while(!Segment(stack.nextToTop(), stack.top()).turnsLeft(Segment(stack.top(), *it))) {
                    stack.pop();
                }
                stack.push(*it);
            }

            this->result = std::move(stack);
            this->p0 = std::nullopt;
            this->sortedPoints = std::nullopt;

            return this->result;
        }
    };
}

#endif //TPFINALAYDAI_ALGOR_GRAHAMSCAN_HPP
