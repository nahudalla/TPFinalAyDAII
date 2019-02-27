#include <algor/SimplePolygon_impl/Edge.hpp>

#include <utility>

using namespace algor::__detail_SimplePolygon;

Edge::Edge(VertexPtr to)
    : _to(std::move(to)), _visited(std::make_shared<bool>(false))
{}

bool Edge::visited() const {
    return *this->_visited;
}

void Edge::visited(bool visited) {
    *this->_visited = visited;
}

VertexPtr Edge::to() const {
    return this->_to;
}

void Edge::to(VertexPtr vertex) {
    this->_to = std::move(vertex);
    this->visited(false);
}

Edge::operator bool() const {
    return (bool) this->_to;
}

void Edge::setDualEdge(Edge::Ptr const &edge) {
    this->_visited = edge->_visited;
}

void Edge::treeNodeAbove(TreeNodePtr node) {
    this->_treeNodeAbove = std::move(node);
}

void Edge::treeNodeBelow(TreeNodePtr node) {
    this->_treeNodeBelow = std::move(node);
}

TreeNodePtr const& Edge::treeNodeAbove() const {
    return this->_treeNodeAbove;
}

TreeNodePtr const& Edge::treeNodeBelow() const {
    return this->_treeNodeBelow;
}
