#ifndef TPFINALAYDAI_ALGOR_SEGMENT_HPP
#define TPFINALAYDAI_ALGOR_SEGMENT_HPP

#include <algorithm>

#include <algor/Point.hpp>
#include <algor/Vector.hpp>
#include <algor/ComparisonResult.hpp>
#include <algor/GeometricObject.hpp>

namespace algor {
    class Segment : public GeometricObject {
        Point from, to;

    protected:
        const Segment *getAsSegment() const override {return this;}

    public:
        GeometricObject *clone() const override {
            return dynamic_cast<GeometricObject *>(new Segment(*this));
        }

        bool isSegment() const override {return true;}

        Segment(const Point &from, const Point &to) : from(from), to(to) {}

        const Point &getFrom() const {
            return from;
        }

        const Point &getTo() const {
            return to;
        }

        bool isClockwise(Segment const &rhs) const {
            if (this->from != rhs.from) return false;

            Vector v1(this->from, this->to), v2(rhs.from, rhs.to);

            return v1.isClockwise(v2);
        }

        bool isColinear(Segment const &rhs) const {
            if (this->from != rhs.from) return false;

            Vector v1(this->from, this->to), v2(rhs.from, rhs.to);

            return v1.isColinear(v2);
        }

        bool isCounterClockwise(Segment const &rhs) const {
            if (this->from != rhs.from) return false;

            Vector v1(this->from, this->to), v2(rhs.from, rhs.to);

            return v1.isCounterClockwise(v2);
        }

        bool turnsLeft(Segment const &rhs) const {
            if (this->to != rhs.from) return false;

            Vector v1(this->from, rhs.from), v2(this->from, rhs.to);

            return v1.isCounterClockwise(v2);
        }

        bool turnsRight(Segment const &rhs) const {
            if (this->to != rhs.from) return false;

            Vector v1(this->from, rhs.from), v2(this->from, rhs.to);

            return v1.isClockwise(v2);
        }

        bool intersects(Segment const &rhs) const {
            auto d1 = Vector(rhs.from, this->from).crossProduct(Vector(rhs.from, rhs.to));
            auto d2 = Vector(rhs.from, this->to).crossProduct(Vector(rhs.from, rhs.to));
            auto d3 = Vector(this->from, rhs.from).crossProduct(Vector(this->from, this->to));
            auto d4 = Vector(this->from, rhs.to).crossProduct(Vector(this->from, this->to));

            return (((d1 > 0 and d2 < 0) or (d1 < 0 and d2 > 0)) and ((d3 > 0 and d4 < 0) or (d3 < 0 and d4 > 0)))
                   || (d1 == 0 and rhs.contains(this->from))
                   || (d2 == 0 and rhs.contains(this->to))
                   || (d3 == 0 and this->contains(rhs.from))
                   || (d4 == 0 and this->contains(rhs.to));
        }

        bool contains(Point point) const {
            return std::min(this->from.getX(), this->to.getX()) <= point.getX()
                   && point.getX() <= std::max(this->from.getX(), this->to.getX())
                   && std::min(this->from.getY(), this->to.getY()) <= point.getY()
                   && point.getY() <= std::max(this->from.getY(), this->to.getY());
        }

        const Point & getLeftmostEndpoint() const {
            if(this->from.getX() < this->to.getX()) {
                return this->from;
            } else {
                return this->to;
            }
        }

        const Point & getRightmostEndpoint() const {
            if(this->from.getX() > this->to.getX()) {
                return this->from;
            } else {
                return this->to;
            }
        }

        ComparisonResult comparePointWithIntersection(const Segment & other, const Point & point) const {
            // PRECONDICION: los segmentos se intersecan
            auto x1 = this->getFrom().getX(),
                 x2 = this->getTo().getX(),
                 x3 = other.getFrom().getX(),
                 x4 = other.getTo().getX();
            auto y1 = this->getFrom().getY(),
                 y2 = this->getTo().getY(),
                 y3 = other.getFrom().getY(),
                 y4 = other.getTo().getY();
            auto x0 = point.getX();

            auto lhs = (x4-x3)*(y2-y1)*x1-(x2-x1)*(y4-y3)*x3+(x2-x1)*(x4-x3)*(y3-y1);
            auto rhs = (x4-x3)*(y2-y1)*x0-(x2-x1)*(y4-y3)*x0;

            return (lhs < rhs) ? LESS : ((lhs == rhs) ? EQUAL : GREATER);
        }

        bool isBelowAtPoint(const Segment & other, const Point & point) const {
            if(this->intersects(other)) {
                auto pointIs = this->comparePointWithIntersection(other, point);

                if(pointIs == EQUAL) return true; // Estan a la misma altura, se consideran ambos arriba y abajo del otro

                if(pointIs == LESS) {
                    return this->getLeftmostEndpoint().getY() < other.getLeftmostEndpoint().getY();
                } else { // pointIs == GREATER
                    return this->getRightmostEndpoint().getY() < other.getRightmostEndpoint().getY();
                }
            } else {
                if(this->getLeftmostEndpoint().getX() < other.getLeftmostEndpoint().getX()) {
                    Vector v1(this->getLeftmostEndpoint(), this->getRightmostEndpoint());
                    Vector v2(this->getLeftmostEndpoint(), other.getLeftmostEndpoint());

                    return v1.isCounterClockwise(v2);
                } else {
                    Vector v1(other.getLeftmostEndpoint(), other.getRightmostEndpoint());
                    Vector v2(other.getLeftmostEndpoint(), this->getLeftmostEndpoint());

                    return v1.isClockwise(v2);
                }
            }
        }

        auto squaredLength() const {
            return Vector(this->from, this->to).squaredLength();
        }

        bool operator==(const Segment &rhs) const {
            return from == rhs.from &&
                   to == rhs.to;
        }

        bool operator!=(const Segment &rhs) const {
            return !(rhs == *this);
        }
    };
}

#endif //TPFINALAYDAI_ALGOR_SEGMENT_HPP
