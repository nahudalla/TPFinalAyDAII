#include <algor/Point.hpp>
#include <algor/Vector.hpp>

using algor::Point;
using algor::Vector;

Vector operator-(Point const& lhs, Point const& rhs){
    return Vector(lhs.getX()-rhs.getX(), lhs.getY()-rhs.getY());
}
