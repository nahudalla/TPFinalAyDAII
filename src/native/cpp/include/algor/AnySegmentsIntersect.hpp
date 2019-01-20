#ifndef TPFINALAYDAI_ALGOR_ANYSEGMENTSINTERSECT_HPP
#define TPFINALAYDAI_ALGOR_ANYSEGMENTSINTERSECT_HPP

#include <optional>
#include <tuple>

#include <algor/List.hpp>
#include <algor/Segment.hpp>
#include <algor/RBTree.hpp>
#include <algor/Comparator.hpp>
#include <algor/ComparisonResult.hpp>

namespace algor {
    class AnySegmentsIntersect {
        enum Position {LEFT, RIGHT};

        struct Endpoint {
            Segment segment;
            Position position;
        };

        static const auto & getComparator() {
            typedef Comparator<Endpoint> TCmp;

            static const TCmp endpoint_comparator = (TCmp::ComparatorFunction)
                [](const Endpoint & lhs, const Endpoint & rhs) -> ComparisonResult {
                    auto & point1 = (lhs.position == LEFT)
                                        ? lhs.segment.getLeftmostEndpoint()
                                        : lhs.segment.getRightmostEndpoint();
                    auto & point2 = (rhs.position == LEFT)
                                        ? rhs.segment.getLeftmostEndpoint()
                                        : rhs.segment.getRightmostEndpoint();

                    if(point1.getX() == point2.getX()) {
                        if(lhs.position == rhs.position) {
                            return point1.getY() == point2.getY() ? EQUAL :
                                    (point1.getY() < point2.getY() ? LESS : GREATER);
                        } else if(lhs.position == LEFT && rhs.position == RIGHT) return LESS;
                        else return GREATER;
                    } else if(point1.getX() < point2.getX()) return LESS;
                    else return GREATER;
            };

            return endpoint_comparator;
        }

        std::optional<bool> result;
        std::optional<List<Endpoint>> endpoints;
    public:
		bool canRun() const {
			return this->endpoints.has_value();
		}
	
        explicit AnySegmentsIntersect(List<Segment> const& segments) {
            // Preparo los puntos para luego ser procesados
            auto it = segments.cbegin();
            auto end = segments.cend();

            this->endpoints = List<Endpoint>();

            for(; it != end; ++it) {
                const Segment & segment = *it;

                if(segment.getFrom().getX() == segment.getTo().getX()) continue; // PRECONDICIÓN: no puede haber segmentos verticales

                endpoints->add(Endpoint{segment, LEFT});
                endpoints->add(Endpoint{segment, RIGHT});
            }

            if(this->endpoints->isEmpty()) {
                this->endpoints.reset();
            } else {
                this->endpoints->sort(AnySegmentsIntersect::getComparator());
            }
        }

        std::optional<bool> run() const {
            return this->result;
        }

        std::optional<bool> run() {
            if(this->result.has_value()) {
                return this->result;
            }

            if(!this->endpoints.has_value() || this->endpoints->isEmpty()) {
                return std::nullopt;
            }

            auto it = this->endpoints->cbegin();
            auto end = this->endpoints->cend();

            Point sweepLinePoint;
            Comparator<Segment>::ComparatorFunction totalOrderComparator =
                    [&sweepLinePoint](const Segment & lhs, const Segment & rhs) -> Comparator<Segment>::Result {
                        return lhs.isBelowAtPoint(rhs, sweepLinePoint) ? LESS :
                                (rhs.isBelowAtPoint(lhs, sweepLinePoint) ? GREATER :
                                 (lhs == rhs ? EQUAL : GREATER));
                                    // Son iguales solo si son el mismo segmento.
                                    // El árbol Red-Black usa sólo menor para ordenar,
                                    // e igual para buscar. Por eso es necesario que
                                    // el comparador devuelva EQUAL sólo cuando son el
                                    // mismo segmento. De todos modos, si no es ni menor
                                    // ni mayor significa que la línea de barrido se encuentra
                                    // en el punto de intersección.
                    };

            RBTree<Segment> tree(std::move(totalOrderComparator));
            auto tree_end = tree.end();

            bool anySegmentsIntersect = false;
            for (; it != end && !anySegmentsIntersect; ++it) {
                const Endpoint & endpoint = *it;
                if(endpoint.position == LEFT) {
                    sweepLinePoint = endpoint.segment.getLeftmostEndpoint();
                    auto it_segment = tree.insert(endpoint.segment);
                    auto above = it_segment; above++;
                    auto below = it_segment; below--;

                    if((above != tree_end && it_segment->intersects(*above))
                        || (below != tree_end && it_segment->intersects(*below))) {
                        anySegmentsIntersect = true;
                    }
                } else {
                    sweepLinePoint = endpoint.segment.getRightmostEndpoint();
                    auto it_segment = tree.search(endpoint.segment);
                    auto above = it_segment; above++;
                    auto below = it_segment; below--;

                    if(above != tree_end && below != tree_end && above->intersects(*below)) {
                        anySegmentsIntersect = true;
                    }
                    tree.remove(it_segment);
                }
            }

            this->result = anySegmentsIntersect;
            this->endpoints = std::nullopt;

            return this->result;
        }
    };
}

#endif //TPFINALAYDAI_ALGOR_ANYSEGMENTSINTERSECT_HPP
