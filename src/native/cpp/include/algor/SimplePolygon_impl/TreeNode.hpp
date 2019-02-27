#ifndef TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_TREENODE_HPP
#define TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_TREENODE_HPP

#include <memory>

#include <algor/List.hpp>

#include <algor/SimplePolygon_impl/Vertex.hpp>

namespace algor::__detail_SimplePolygon {
    typedef std::shared_ptr<TreeNode> TreeNodePtr;

    class TreeNode : public std::enable_shared_from_this<TreeNode> {
        static std::size_t getNextID();

        const std::size_t _id = getNextID();

    public:
        typedef TreeNodePtr Ptr;

    private:
        VertexPtr _v1, _v2, _v3;
        bool _visited = false;
        List<TreeNodePtr> _neighbours;

        TreeNode(VertexPtr v1, VertexPtr v2, VertexPtr v3);

        void _addNeighbour(TreeNodePtr);

    public:
        static TreeNodePtr create(VertexPtr v1, VertexPtr v2, VertexPtr v3);

        VertexPtr vertexA() const;
        VertexPtr vertexB() const;
        VertexPtr vertexC() const;

        bool visited() const;
        void visited(bool);

        void connect(TreeNodePtr);
        List<TreeNodePtr> const& neighbours() const;
        bool isNeighbour(TreeNodePtr const&) const;

        std::size_t id() const;

        void colorize(List<Point> &, List<Point> &, List<Point> &);

        bool operator==(const TreeNode &rhs) const;

        bool operator!=(const TreeNode &rhs) const;
    };
}

#endif //TPFINALAYDAII_ALGOR_SIMPLE_POLYGON_IMPL_TREENODE_HPP
