#ifndef TPFINALAYDAI_GEOMETRICOBJECT_HPP
#define TPFINALAYDAI_GEOMETRICOBJECT_HPP

#include <logger/Logger.hpp>

namespace algor {
    class Point;
    class Segment;
    class Vector;

    class GeometricObject {
    protected:
        virtual const Point * getAsPoint() const {return nullptr;};
        virtual const Segment * getAsSegment() const {return nullptr;};
        virtual const Vector * getAsVector() const {return nullptr;};
    public:
        GeometricObject() = default;
        GeometricObject(GeometricObject const&) = default;
        GeometricObject &operator=(GeometricObject const&) = default;
        GeometricObject(GeometricObject &&) noexcept = default;
        GeometricObject &operator=(GeometricObject &&) noexcept = default;
        virtual ~GeometricObject() = default;

        virtual bool isPoint() const {return false;}
        virtual bool isSegment() const {return false;}
        virtual bool isVector() const {return false;}

        virtual GeometricObject * clone() const = 0;

        Point * asPoint() {
            if(!this->isPoint()) {
                logger::log(
                        logger::flags::CRITICAL,
                        "Invalid conversion. GeometricObject is not a Point"
                );
                return nullptr;
            }

            return const_cast<Point *>(this->getAsPoint());
        }
        const Point * asPoint() const {
            if(!this->isPoint()) {
                logger::log(
                        logger::flags::CRITICAL,
                        "Invalid conversion. GeometricObject is not a Point"
                );
                return nullptr;
            }

            return this->getAsPoint();
        }

        Segment * asSegment() {
            if(!this->isSegment()) {
                logger::log(
                        logger::flags::CRITICAL,
                        "Invalid conversion. GeometricObject is not a Segment"
                );
                return nullptr;
            }

            return const_cast<Segment *>(this->getAsSegment());
        }
        const Segment * asSegment() const {
            if(!this->isSegment()) {
                logger::log(
                        logger::flags::CRITICAL,
                        "Invalid conversion. GeometricObject is not a Segment"
                );
                return nullptr;
            }

            return this->getAsSegment();
        }

        Vector * asVector() {
            if(!this->isVector()) {
                logger::log(
                        logger::flags::CRITICAL,
                        "Invalid conversion. GeometricObject is not a Vector"
                );
                return nullptr;
            }

            return const_cast<Vector *>(this->getAsVector());
        }
        const Vector * asVector() const {
            if(!this->isVector()) {
                logger::log(
                        logger::flags::CRITICAL,
                        "Invalid conversion. GeometricObject is not a Vector"
                );
                return nullptr;
            }

            return this->getAsVector();
        }
    };
}

#endif //TPFINALAYDAI_GEOMETRICOBJECT_HPP
