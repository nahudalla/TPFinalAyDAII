#ifndef TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_VERTEX_HPP
#define TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_VERTEX_HPP

#include <memory>
#include <utility>

#include <algor/Array.hpp>
#include <algor/Point.hpp>
#include <algor/RBTree.hpp>
#include <algor/Segment.hpp>
#include <algor/SimplePolygon_impl/Edge.hpp>

namespace algor::__detail_SimplePolygon {
    class TreeNode;
    typedef std::shared_ptr<TreeNode> TreeNodePtr;

    class Vertex;
    typedef std::shared_ptr<Vertex> VertexPtr;

    // Realiza un merge sort sobre los elementos del arreglo de vértices, pero guarda el resultado del ordenamiento
    // en la lista vinculada interna de los vértices (nextBelow y previousAbove)
    VertexPtr merge_sort(Array<VertexPtr> & vertices, std::size_t begin, std::size_t end);

    class Vertex : public std::enable_shared_from_this<Vertex> {
    public:
        typedef std::shared_ptr<Vertex> VertexPtr;
        typedef VertexPtr Ptr;

        enum Color {COLOR_A, COLOR_B, COLOR_C, UNDEFINED};

    private:
        Point _position;
        Edge::Ptr _prev, _next;

        // Linked list from top to bottom, left to right
        VertexPtr _nextBelow, _prevAbove;

        VertexPtr _helper;

        bool _visited = false;

        Color _color = UNDEFINED;

        RBTree<Edge::Ptr> _diagonals = RBTree<Edge::Ptr>(
                // Ordeno las diagonales que **salen desde este vértice hacia abajo**, de menor a mayor según el
                // ángulo que forman con el eje X positivo.
                // Estas diagonales luego serán usadas en este órden para armar los polígonos monónotonos con respecto
                // al eje Y.
                (Comparator<Edge::Ptr>::ComparatorFunction)
                [this](Edge::Ptr const& d1, Edge::Ptr const& d2)->Comparator<Edge::Ptr>::Result{
                    typedef Comparator<Edge::Ptr>::Result Result;

                    if(d1->to()->position() == d2->to()->position()) return Result::EQUAL;

                    if(Segment(d1->to()->position(), this->position())
                        .turnsLeft(Segment(this->position(), d2->to()->position()))
                    ) { return Result::LESS; }
                    else { return Result::GREATER; }
                }
        );

        RBTree<Edge::Ptr> _triangulationEdges = RBTree<Edge::Ptr>(
                // Ordeno las diagonales que forman los triángulos de menor a mayor,
                // según el vértice de destino
                (Comparator<Edge::Ptr>::ComparatorFunction)
                        [this](Edge::Ptr const& d1, Edge::Ptr const& d2)->Comparator<Edge::Ptr>::Result{
                            typedef Comparator<Edge::Ptr>::Result Result;

                            if((*d1->to()) == (*d2->to())) { return Result::EQUAL; }
                            else if((*d1->to()) < (*d2->to())){ return Result::LESS; }
                            else { return Result::GREATER; }
                        }
        );

        explicit Vertex(Point position);
    public:
        static VertexPtr create(Point position) {
            return VertexPtr(new Vertex(std::move(position)));
        }

        enum Type {START, END, MERGE, SPLIT, REGULAR_LEFT, REGULAR_RIGHT, UNKNOWN};

        Vertex(Vertex const&)                 = delete;
        Vertex &operator=(Vertex const&)      = delete;
        Vertex(Vertex &&) noexcept            = delete;
        Vertex &operator=(Vertex &&) noexcept = delete;

        ~Vertex() = default;

        bool operator==(Vertex const&) const;
        bool operator!=(Vertex const&) const;
        bool operator<(Vertex const&) const;
        bool operator<=(Vertex const&) const;
        bool operator>(Vertex const&) const;
        bool operator>=(Vertex const&) const;

        auto x() const {return this->_position.getX();}
        auto y() const {return this->_position.getY();}
        Point position() const { return this->_position; }

        void previous(VertexPtr);
        Edge::Ptr previous() const;

        void next(VertexPtr);
        Edge::Ptr next() const;

        void nextBelow(VertexPtr);
        VertexPtr nextBelow() const;
        void previousAbove(VertexPtr);
        VertexPtr previousAbove() const;
        bool canBeRemoved() const;
        void attemptRemoveFromOrderedList();

        std::pair<VertexPtr, VertexPtr> visitStart();
        VertexPtr visitLeftChain();
        VertexPtr visitRightChain();

        Type type() const;

        Edge::Ptr addTriangulationEdge(VertexPtr);
        Edge::Ptr findTriangulationEdge(VertexPtr);

        void addDiagonal(VertexPtr);
        RBTree<Edge::Ptr> & diagonals();
        RBTree<Edge::Ptr> const& diagonals() const;
        void visitDiagonal(RBTree<Edge::Ptr>::Iterator &);

        Segment edgeSegment() const;

        void helper(VertexPtr);
        VertexPtr helper() const;

        bool visited() const;
        void visited(bool);

        Color color() const;
        void color(Color);

        void reset();
    };
}

#endif //TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_VERTEX_HPP
