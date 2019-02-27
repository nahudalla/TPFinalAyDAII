#include <algor/Segment.hpp>
#include <algor/Vector.hpp>
#include <algor/Point.hpp>
#include <algor/Array.hpp>

#include <algor/SimplePolygon_impl/Edge.hpp>
#include <algor/SimplePolygon_impl/Vertex.hpp>

using namespace algor;
using namespace algor::__detail_SimplePolygon;


Vertex::Vertex(Point position)
    : _position(std::move(position)), _next(Edge::create()), _prev(Edge::create())
{}

bool Vertex::operator<(Vertex const & other) const {
    return other._position.getY() > this->_position.getY()
            || (other._position.getY() == this->_position.getY()
                && other._position.getX() < this->_position.getX());
}

bool Vertex::operator>(Vertex const & other) const {
    return other < *this;
}

bool Vertex::operator==(Vertex const & other) const {
    return this->position() == other.position();
}

bool Vertex::operator!=(Vertex const & other) const {
    return !(*this == other);
}

bool Vertex::operator<=(Vertex const & other) const {
    return !(*this > other);
}

bool Vertex::operator>=(Vertex const & other) const {
    return !(*this < other);
}

void Vertex::previous(Vertex::VertexPtr previous) {
    previous->_next->to(this->shared_from_this());
    previous->_next->setDualEdge(this->_prev);
    this->_prev->to(std::move(previous));
}

Edge::Ptr Vertex::previous() const {
    return this->_prev;
}

void Vertex::next(Vertex::VertexPtr next) {
    next->_prev->to(this->shared_from_this());
    next->_prev->setDualEdge(this->_next);
    this->_next->to(std::move(next));
}

Edge::Ptr Vertex::next() const {
    return this->_next;
}

void Vertex::nextBelow(VertexPtr next) {
    this->_nextBelow = std::move(next);
}

Vertex::VertexPtr Vertex::nextBelow() const {
    return this->_nextBelow;
}

void Vertex::previousAbove(VertexPtr prev) {
    this->_prevAbove = std::move(prev);
}

Vertex::VertexPtr Vertex::previousAbove() const {
    return this->_prevAbove;
}

bool isStartVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next);
bool isSplitVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next);
bool isEndVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next);
bool isMergeVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next);
bool isRegularLeftVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next);
bool isRegularRightVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next);

Vertex::Type Vertex::type() const {
    // TODO: test

    if(!this->_prev || !this->_next) return UNKNOWN;

    auto & prev = *(this->_prev->to());
    auto & next = *(this->_next->to());

    if(isStartVertex(*this, prev, next)) return START;
    if(isSplitVertex(*this, prev, next)) return SPLIT;
    if(isEndVertex(*this, prev, next)) return END;
    if(isMergeVertex(*this, prev, next)) return MERGE;
    if(isRegularLeftVertex(*this, prev, next)) return REGULAR_LEFT;
    if(isRegularRightVertex(*this, prev, next)) return REGULAR_RIGHT;

    return UNKNOWN;
}

bool isStartVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next)
{
    return vertex > prev && vertex > next
            && Segment(prev.position(), vertex.position())
            .turnsLeft(
                    Segment(vertex.position(), next.position())
            );
}

bool isSplitVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next)
{
    return vertex > prev && vertex > next
           && Segment(prev.position(), vertex.position())
           .turnsRight(
                   Segment(vertex.position(), next.position())
           );
}

bool isEndVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next)
{
    return vertex < prev && vertex < next
           && Segment(prev.position(), vertex.position())
           .turnsLeft(
                   Segment(vertex.position(), next.position())
           );
}

bool isMergeVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next)
{
    return vertex < prev && vertex < next
           && Segment(prev.position(), vertex.position())
           .turnsRight(
                   Segment(vertex.position(), next.position())
           );
}

bool isRegularLeftVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next)
{
    return prev > vertex && next < vertex;
}

bool isRegularRightVertex(Vertex const& vertex, Vertex const& prev, Vertex const& next)
{
    return prev < vertex && next > vertex;
}

Edge::Ptr Vertex::addTriangulationEdge(VertexPtr other) {
    if(*this < *other) {
        return other->addTriangulationEdge(this->shared_from_this());
    } else {
//        std::cout << "\tADD_TRIANG_EDGE: (" << this->position() << ") -- (" << other->position() << ")";
        auto e = Edge::create(other);
        auto it = this->_triangulationEdges.search(e);

        if(it == this->_triangulationEdges.end()) {
//            std::cout << " ADDED" << std::endl;
            this->_triangulationEdges.insert(e);
            return std::move(e);
        } else {
//            std::cout << " EXISTS" << std::endl;
            return *it;
        }
    }
}

Edge::Ptr Vertex::findTriangulationEdge(VertexPtr vertex) {
    auto it = this->_triangulationEdges.search(Edge::create(std::move(vertex)));

    if(it == this->_triangulationEdges.end()) {
        return nullptr;
    }

    return *it;
}

void Vertex::addDiagonal(Vertex::VertexPtr other) {
    // Agrego la diagonal desde el vértice que está más arriba hacia el que está más abajo

    if(*this < *other) {
        other->addDiagonal(this->shared_from_this());
        return;
    }

    this->_diagonals.insert(Edge::create(other));

    // También la agrego como parte de la triangulación
    this->addTriangulationEdge(std::move(other));
}

void Vertex::visitDiagonal(RBTree<Edge::Ptr>::Iterator &itDiag) {
    if(itDiag == this->_diagonals.end()) return;

    if((*itDiag)->visited()) {
        this->_diagonals.remove(itDiag);
        (*itDiag)->visited(false);
    }
}

VertexPtr merge(VertexPtr vertex1, VertexPtr vertex2);

Vertex::VertexPtr algor::__detail_SimplePolygon::merge_sort(
        Array<Vertex::VertexPtr> & vertices,
        std::size_t begin,
        std::size_t end
) {
    // TODO: test

    if(begin == end) return nullptr;

    // Si hay menos de dos elementos, ya está ordenada.
    // Limpio el vértice actual de posibles residuos de ejecuciones anteriores y devuelvo
    if(end-begin < 2) {
        auto list = vertices[begin];
        list->reset();
        return std::move(list);
    }

    // Split (Divide)
    std::size_t middle = begin + ((end-begin) / 2);

    // Sort
    auto list1 = merge_sort(vertices, begin, middle);
    auto list2 = merge_sort(vertices, middle, end);

    if(!list1) return std::move(list2);
    if(!list2) return std::move(list1);

    return merge(list1, list2);
}

Vertex::VertexPtr merge(Vertex::VertexPtr vertex1, Vertex::VertexPtr vertex2) {
    VertexPtr head;

    if(*vertex1 > *vertex2) {
        head = vertex1;
        vertex1 = vertex1->nextBelow();
    } else {
        head = vertex2;
        vertex2 = vertex2->nextBelow();
    }

    auto current = head;
    while(vertex1 && vertex2) {
        if(*vertex1 > *vertex2) {
            current->nextBelow(vertex1);
            vertex1->previousAbove(current);

            current = vertex1;

            vertex1 = vertex1->nextBelow();
        } else {
            current->nextBelow(vertex2);
            vertex2->previousAbove(current);

            current = vertex2;

            vertex2 = vertex2->nextBelow();
        }
    }

    if(vertex1) {
        current->nextBelow(vertex1);
        vertex1->previousAbove(current);
    }

    if(vertex2) {
        current->nextBelow(vertex2);
        vertex2->previousAbove(current);
    }

    return head;
}

Segment Vertex::edgeSegment() const {
    return Segment(this->_position, this->_next->to()->_position);
}

void Vertex::helper(VertexPtr helper) {
    this->_helper = std::move(helper);
}

VertexPtr Vertex::helper() const {
    return this->_helper;
}

RBTree<Edge::Ptr> &Vertex::diagonals() {
    return this->_diagonals;
}

RBTree<Edge::Ptr> const &Vertex::diagonals() const {
    return this->_diagonals;
}

bool Vertex::visited() const {
    return this->_visited;
}

void Vertex::visited(bool visited) {
    this->_visited = visited;
}

bool Vertex::canBeRemoved() const {
    return this->visited() && this->diagonals().isEmpty() && this->_prev->visited() && this->_next->visited();
}

// TODO: test
std::pair<VertexPtr, VertexPtr> Vertex::visitStart() {
    this->visited(true);

    // TODAS LAS DIAGONALES ESTÁN POR DEBAJO DE ESTE VÉRTICE Y PUEDO VISITAR SÓLO
    // MI ANTERIOR Y SIGUIENTE QUE ESTÉN POR DEBAJO

    Edge::Ptr CD, CI;

    switch (this->type()) {
        case START: {
            if(this->_prev->visited()) {
                // No puedo ir a mi anterior, tiene que haber al menos una diagonal
                if(!this->diagonals().isEmpty()) {
                    // La cadena derecha es la diagonal de mínimo ángulo
                    auto itDiagCD = this->diagonals().minimum();

                    // La potencial cadena izquierda es la diagonal que sigue a CD
                    auto itDiagCI = itDiagCD;
                    ++itDiagCI;

                    CD = *itDiagCD;
                    this->visitDiagonal(itDiagCD);

                    if(itDiagCI == this->diagonals().end()) {
                        // Si no hay diagonal para usar como cadena izquierda, uso mi siguiente
                        CI = this->_next;
                    } else {
                        // Si hay diagonal para usar como cadena izquierda, la uso
                        CI = *itDiagCI;
                        this->visitDiagonal(itDiagCI);
                    }
                }
            } else {
                // Puedo ir a mi anterior
                CD = this->previous();

                if(this->diagonals().isEmpty()) {
                    // Si no hay diagonales, mi siguiente es la CI
                    CI = this->next();
                } else {
                    // Si hay diagonales, la cadena izquierda es la mínima de ellas
                    auto itDiagCI = this->diagonals().minimum();

                    CI = *itDiagCI;
                    this->visitDiagonal(itDiagCI);
                }
            }

            break;
        }
        case MERGE: {
            if(!this->diagonals().isEmpty()) {
                // La diagonal mínima es CD
                auto itDiagCD = this->diagonals().minimum();

                // La diagonal siguiente a la CD es la CI
                auto itDiagCI = itDiagCD;
                ++itDiagCI;

                // La cadena izquierda tiene que existir
                if(itDiagCI != this->diagonals().end()) {
                    CD = *itDiagCD;
                    this->visitDiagonal(itDiagCD);

                    CI = *itDiagCI;
                    this->visitDiagonal(itDiagCI);
                }
            }

            break;
        }
        case SPLIT: {
            // Tiene que haber al menos una diagonal para que pueda ser visitado como START en una particion
            if(!this->diagonals().isEmpty()) {
                auto itMinDiag = this->diagonals().minimum();
                const auto & minDiagTo = (*itMinDiag)->to();

                if(Segment(minDiagTo->position(), this->position())
                    .turnsLeft(Segment(this->position(), this->next()->to()->position()))
                ) {
                    // Si la diagonal minima tiene ángulo menor que mi siguiente con respecto al eje X positivo,
                    // ésta es la CD. La diagonal que le sigue es potencialmente la CI
                    auto itDiagCI = itMinDiag;
                    ++itDiagCI;

                    CD = *itMinDiag;
                    this->visitDiagonal(itMinDiag);

                    if(itDiagCI == this->diagonals().end()
                        || Segment(this->next()->to()->position(), this->position())
                            .turnsLeft(Segment(this->position(), (*itDiagCI)->to()->position()))
                    ) {
                        // Si no hay diagonal siguiente a la mínima o mi vértice siguiente tiene un menor ángulo, elijo
                        // mi siguiente como CI
                        CI = this->next();
                    } else {
                        // Sino, la diagonal siguiente a la mínima es la CI
                        CI = *itDiagCI;
                        this->visitDiagonal(itDiagCI);
                    }
                } else {
                    // Mi siguiente tiene menor ángulo que la mínima de las diagonales, entonces
                    // mi anterior es CD y la mínima de las diagonales es CI
                    CD = this->previous();

                    CI = *itMinDiag;
                    this->visitDiagonal(itMinDiag);
                }
            }

            break;
        }
        case REGULAR_LEFT: {
            // Tiene que haber al menos una diagonal
            if(!this->diagonals().isEmpty()) {
                // La diagonal mínima es la CD, y su siguiente diagonal es una posible cadena izquierda
                auto itDiagCD = this->diagonals().minimum();
                auto itDiagCI = itDiagCD;
                ++itDiagCI;

                CD = *itDiagCD;
                this->visitDiagonal(itDiagCD);

                // Si hay diagonal siguiente a la CD, ésta es la CI. Sino, la CI es mi siguiente
                if(itDiagCI != this->diagonals().end()) {
                    CI = *itDiagCI;
                    this->visitDiagonal(itDiagCI);
                } else {
                    CI = this->next();
                }
            }

            break;
        }
        case REGULAR_RIGHT: {
            // Tiene que haber al menos una diagonal
            if(!this->diagonals().isEmpty()) {
                auto itMinDiag = this->diagonals().minimum();

                // Si puedo ir a mi anterior, éste es la CD y la diagonal mínima es la CI
                if(!this->previous()->visited()) {
                    CD = this->previous();
                    CI = *itMinDiag;
                    this->visitDiagonal(itMinDiag);
                } else {
                    // Sino, la diagonal mínima es CD y tiene que haber una diagonal siguiente que es la CI

                    auto itDiagCI = itMinDiag;
                    ++itDiagCI;

                    if(itDiagCI != this->diagonals().end()) {
                        CD = *itMinDiag;
                        this->visitDiagonal(itMinDiag);
                        CI = *itDiagCI;
                        this->visitDiagonal(itDiagCI);
                    }
                }
            }

            break;
        }

        // No pueden ser visitados como START en una partición
        case END:
        case UNKNOWN:
            break;
    }

    // Las cadenas seleccionadas no pueden estar visitadas.
    // Si lo etán, el nodo actual no podía ser visitado como START en una partición
    if(CD && CD->visited()) CD.reset();
    if(CI && CI->visited()) CI.reset();

    // Si no se pudo seleccionar alguna de las dos cadenas,
    // el nodo no podia ser visitado como START en una partición
    if(!CD || !CI) {
        CD.reset();
        CI.reset();
    } else {
        // Las marco como visitadas
        CI->visited(true);
        CD->visited(true);
    }

    return std::make_pair(CI->to(), CD->to());
}

// TODO: test
VertexPtr Vertex::visitLeftChain() {
    this->visited(true);

    Edge::Ptr next;

    switch (this->type()) {
        case MERGE: {
            // La diagonal mínima tiene que existir y es el siguiente
            if(!this->diagonals().isEmpty()) {
                auto itNextDiag = this->diagonals().minimum();
                next = *itNextDiag;
                this->visitDiagonal(itNextDiag);
            }

            break;
        }
        case SPLIT: {
            // Si la diagonal mínima existe y tiene ángulo menor que mi siguiente,
            // ésta es la siguiente en la cadena. Sino, mi siguiente es el siguiente
            // en la cadena.

            auto itMinDiag = this->diagonals().minimum();

            if(itMinDiag != this->diagonals().end()
                && Segment((*itMinDiag)->to()->position(), this->position())
                .turnsLeft(Segment(this->position(), this->next()->to()->position()))
            ) {
                next = *itMinDiag;
                this->visitDiagonal(itMinDiag);
            } else {
                next = this->next();
            }

            break;
        }
        case REGULAR_LEFT: {
            // Si la diagonal mínima existe, es la siguiente en la cadena.
            // Sino, mi siguiente es el siguiente en la cadena.

            if(!this->diagonals().isEmpty()) {
                auto itDiag = this->diagonals().minimum();
                next = *itDiag;
                this->visitDiagonal(itDiag);
            } else {
                next = this->next();
            }

            break;
        }


        // No pueden ser visitados como parte de la cadena izquierda
        case START:
        case END:
        case REGULAR_RIGHT:
        case UNKNOWN:
            break;
    }

    // El nodo siguiente no puede estar visitado
    // Si lo está, el nodo actual no podía ser
    // visitado como parte de la cadena izquierda
    if(next) {
        if (next->visited()) next.reset();
        else next->visited(true);

        if(next) return next->to();
    }

    return nullptr;
}

// TODO: test
VertexPtr Vertex::visitRightChain() {
    this->visited(true);

    Edge::Ptr next;

    switch (this->type()) {
        case MERGE: {
            // La diagonal máxima es la siguiente en la cadena
            if(!this->diagonals().isEmpty()) {
                auto itDiag = this->diagonals().maximum();
                next = *itDiag;
                this->visitDiagonal(itDiag);
            }

            break;
        }
        case SPLIT: {
            // Si la diagonal máxima existe y tiene ángulo mayor que mi anterior, es la siguiente en la cadena.
            // Sino, mi anterior es el siguiente en la cadena.
            auto itDiag = this->diagonals().maximum();

            if(itDiag != this->diagonals().end()
                && Segment(this->previous()->to()->position(), this->position())
                    .turnsLeft(Segment(this->position(), (*itDiag)->to()->position()))
            ) {
                next = *itDiag;
                this->visitDiagonal(itDiag);
            } else {
                next = this->previous();
            }

            break;
        }
        case REGULAR_RIGHT: {
            // Si la diagonal máxima existe, es la siguiente en la cadena.
            // Sino, mi anterior es el siguiente en la cadena.
            auto itDiag = this->diagonals().maximum();

            if(itDiag != this->diagonals().end()) {
                next = *itDiag;
                this->visitDiagonal(itDiag);
            } else {
                next = this->previous();
            }

            break;
        }

        // No pueden ser visitados como parte de la cadena derecha
        case START:
        case END:
        case REGULAR_LEFT:
        case UNKNOWN:
            break;
    }

    // El nodo siguiente no puede estar visitado
    // Si lo está, el nodo actual no podía ser
    // visitado como parte de la cadena izquierda
    if(next) {
        if (next->visited()) next.reset();
        else next->visited(true);

        return next->to();
    }

    return nullptr;
}

Vertex::Color Vertex::color() const {
    return this->_color;
}

void Vertex::color(Vertex::Color color) {
    this->_color = color;
}

// TODO: remove COUT
#include <iostream>

void Vertex::attemptRemoveFromOrderedList() {
//    std::cout << "ATTEMPTING REMOVE (" << this->position() << "): ";
    if(this->canBeRemoved()) {
//        std::cout << "Removed." << std::endl;
        if(this->previousAbove()) {
            this->previousAbove()->nextBelow(this->nextBelow());
        }
        if(this->nextBelow()) {
            this->nextBelow()->previousAbove(this->previousAbove());
        }
        this->previousAbove(nullptr);
        this->nextBelow(nullptr);
    } else {
        /*std::cout << "Cannot be removed. "
            << this->visited() << " "
            << this->diagonals().isEmpty() << " "
            << this->previous()->visited() << " "
            << this->next()->visited() << std::endl;*/
        this->visited(false);
    }
}

void Vertex::reset() {
    this->_prev->visited(false);
    this->_next->visited(false);
    this->_color = UNDEFINED;
    this->_diagonals.clear();
    this->_triangulationEdges.clear();
    this->_visited = false;
    this->_helper = nullptr;
    this->_prevAbove = nullptr;
    this->_nextBelow = nullptr;
}
