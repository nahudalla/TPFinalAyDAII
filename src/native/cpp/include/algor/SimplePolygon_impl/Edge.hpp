#ifndef TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_DIAGONAL_HPP
#define TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_DIAGONAL_HPP

#include <memory>

namespace algor::__detail_SimplePolygon {
    class Vertex;
    typedef std::shared_ptr<Vertex> VertexPtr;

    class TreeNode;
    typedef std::shared_ptr<TreeNode> TreeNodePtr;

    class Edge : public std::enable_shared_from_this<Edge> {
        VertexPtr _to;
        std::shared_ptr<bool> _visited;

        TreeNodePtr _treeNodeAbove, _treeNodeBelow;

        explicit Edge(VertexPtr to);
    public:
        typedef std::shared_ptr<Edge> Ptr;

        static Ptr create(VertexPtr to = nullptr) {
            return std::shared_ptr<Edge>(new Edge(std::move(to)));
        }

        void setDualEdge(Edge::Ptr const& edge);

        Edge()                            = delete;
        Edge(Edge const&)                 = default;
        Edge &operator=(Edge const&)      = default;
        Edge(Edge &&) noexcept            = default;
        Edge &operator=(Edge &&) noexcept = default;
        ~Edge()                           = default;

        bool visited() const;
        void visited(bool);

        void to(VertexPtr);
        VertexPtr to() const;

        explicit operator bool() const;

        void treeNodeAbove(TreeNodePtr);
        void treeNodeBelow(TreeNodePtr);
        TreeNodePtr const& treeNodeAbove() const;
        TreeNodePtr const& treeNodeBelow() const;
    };
}

#endif //TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_DIAGONAL_HPP
