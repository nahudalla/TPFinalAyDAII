#ifndef TPFINALAYDAI_JARVISMARCH_HPP
#define TPFINALAYDAI_JARVISMARCH_HPP

#include <algor/Stack.hpp>
#include <algor/Point.hpp>
#include <algor/Vector.hpp>
#include <algor/Segment.hpp>
#include <algor/Comparator.hpp>

#include <optional>

namespace algor {
    class JarvisMarch {
        std::optional<Stack<Point>> result;
        std::optional<List<Point>> points;
        std::optional<Point> highest;
        std::optional<Point> lowest;

        typedef Comparator<Point>::ComparatorFunction CmpFn;
        typedef Comparator<Point>::Result CmpRes;

    public:
        bool hasResult() const { return this->result.has_value(); }
        bool canRun() const {
            return this->points.has_value() && this->highest.has_value() && this->lowest.has_value();
        }

        explicit JarvisMarch(List<Point> points) {
            {
                // PRECONDICION: La lista tiene que tener al menos tres puntos
                auto it1 = points.begin();
                auto it2 = it1; ++it2;
                auto it3 = it2; ++it3;
                auto end = points.end();

                if(it1 == end || it2 == end || it3 == end) {
                    return;
                }
            }

            {
                auto it = points.findMinimum((CmpFn) [](const Point &lhs, const Point &rhs) -> CmpRes {
                    if(lhs.getY() < rhs.getY()) return CmpRes::LESS;
                    else if(rhs.getY() < lhs.getY()) return CmpRes::GREATER;
                    else if(lhs.getX() < rhs.getX()) return CmpRes::LESS;
                    else if(rhs.getX() < lhs.getX()) return CmpRes::GREATER;
                    else return CmpRes::EQUAL;
                });

                this->lowest = *it;
            }

            {
                auto it = points.findMinimum((CmpFn) [](const Point &lhs, const Point &rhs) -> CmpRes {
                    if(lhs.getY() > rhs.getY()) return CmpRes::LESS;
                    else if(rhs.getY() > lhs.getY()) return CmpRes::GREATER;
                    else if(lhs.getX() > rhs.getX()) return CmpRes::LESS;
                    else if(rhs.getX() > lhs.getX()) return CmpRes::GREATER;
                    else return CmpRes::EQUAL;
                });

                this->highest = *it;
            }

            this->points = std::move(points);
        }

        std::optional<Stack<Point>> run() const { return this->result; }

        std::optional<Stack<Point>> run() {
            if(this->hasResult()) return this->result;

            if(!this->canRun()) return std::nullopt;

            Stack<Point> result;

            const Point & last = *(this->lowest),
                  & highest = *(this->highest);

            Point current = last;

            bool highestReached = false;

            do {
                if(current == highest) highestReached = true;

                result.push(current);

                auto it_next = this->points->findMinimum(
                        (CmpFn) [&current, &highestReached](const Point& lhs, const Point& rhs) -> CmpRes {
                            if(lhs == rhs) return CmpRes::EQUAL;

                            Point _current, _lhs, _rhs;

                            if(highestReached) { // Si se alcanzó el punto más alto, trabajo con los ejes invertidos
                                _current = Point(-current.getX(), -current.getY());
                                _lhs = Point(-lhs.getX(), -lhs.getY());
                                _rhs = Point(-rhs.getX(), -rhs.getY());
                            } else {
                                _current = current;
                                _lhs = lhs;
                                _rhs = rhs;
                            }

                            Vector v1(_current, _lhs),
                                   v2(_current, _rhs);

                            if(v1.isCounterClockwise(v2)) return CmpRes::LESS;
                            else if(v1.isClockwise(v2)) return CmpRes::GREATER;
                            else if(Segment(_current, _lhs).contains(_rhs)) return CmpRes::LESS;
                            else return CmpRes::GREATER;
                        }
                );

                current = this->points->remove(it_next);
            }while(current != last);

            this->result = result;
            this->highest = std::nullopt;
            this->lowest = std::nullopt;
            this->points = std::nullopt;

            return this->result;
        }
    };
}

#endif //TPFINALAYDAI_JARVISMARCH_HPP
