#ifndef TPFINALAYDAI_ALGOR_VECTOR_HPP
#define TPFINALAYDAI_ALGOR_VECTOR_HPP

#include <algor/Point.hpp>
#include <cstddef>
#include <cmath>

#include <algor/GeometricObject.hpp>

namespace algor {
    class Vector : public Point {
    protected:
        const Vector *getAsVector() const override {return this;}

    public:
        GeometricObject *clone() const override {
            return dynamic_cast<GeometricObject *>(new Vector(*this));
        }

        bool isPoint() const override {return false;}
        bool isVector() const override {return true;}

        Vector() = default;

        Vector(int x, int y) : Point(x, y) {}

        explicit Vector(Point &&point) : Point(point) {}

        explicit Vector(Point const &point) : Point(point) {}

        Vector(Point const &from, Point const &to) : Point(to.getX() - from.getX(), to.getY() - from.getY()) {}

        Vector(Vector const &) = default;

        Vector(Vector &&) noexcept = default;

        Vector &operator=(Vector const &) = default;

        Vector &operator=(Vector &&) noexcept = default;

        ~Vector() override = default;

        int crossProduct(Vector const &rhs) const {
            return (this->getX() * rhs.getY()) - (rhs.getX() * this->getY());
        }

        bool isClockwise(Vector const &rhs) const {
            return rhs.crossProduct(*this) > 0;
        }

        bool isColinear(Vector const &rhs) const {
            return rhs.crossProduct(*this) == 0;
        }

        bool isCounterClockwise(Vector const &rhs) const {
            return rhs.crossProduct(*this) < 0;
        }

        auto squaredLength() {
            return (std::size_t) this->getX()*this->getX() + this->getY()*this->getY();
        }
    };
}

algor::Vector operator-(algor::Point const& lhs, algor::Point const& rhs);

#endif //TPFINALAYDAI_ALGOR_VECTOR_HPP
