
#include <algor/SimplePolygon_impl/TreeNode.hpp>

using namespace algor;
using namespace algor::__detail_SimplePolygon;

TreeNode::TreeNode(VertexPtr v1, VertexPtr v2, VertexPtr v3)
    : _v1(std::move(v1)), _v2(std::move(v2)), _v3(std::move(v3))
{}

std::size_t TreeNode::id() const {
    return this->_id;
}

void connectTree(TreeNodePtr & node, VertexPtr const& max, VertexPtr const& mid, VertexPtr const& min) {
    bool midIsLeft = Segment(min->position(), max->position())
            .turnsLeft(Segment(max->position(), mid->position()));

    auto edgeMaxMin = max->addTriangulationEdge(min);

    if((midIsLeft && min->position().getX() <= max->position().getX())
        || (!midIsLeft && min->position().getX() > max->position().getX())
    ) {
//        std::cout << "\tABOVE (" << max->position() << ") -- (" << min->position() << ")" << std::endl;
        // Estoy arriba de la arista max -- min
        edgeMaxMin->treeNodeAbove(node);
        if (edgeMaxMin->treeNodeBelow()) {
            node->connect(edgeMaxMin->treeNodeBelow());
        }
    } else {
//        std::cout << "\tBELOW (" << max->position() << ") -- (" << min->position() << ")" << std::endl;
        // Estoy abajo de la arista max -- min
        edgeMaxMin->treeNodeBelow(node);
        if (edgeMaxMin->treeNodeAbove()) {
            node->connect(edgeMaxMin->treeNodeAbove());
        }
    }

    auto edgeMaxMid = max->addTriangulationEdge(mid);

    if((midIsLeft && mid->position().getX() > max->position().getX())
        || (!midIsLeft && mid->position().getX() <= max->position().getX())) {
//        std::cout << "\tABOVE (" << max->position() << ") -- (" << mid->position() << ")" << std::endl;
        // estoy abajo de max -- mid
        edgeMaxMid->treeNodeAbove(node);
        if (edgeMaxMid->treeNodeBelow()) {
            node->connect(edgeMaxMid->treeNodeBelow());
        }
    } else {
//        std::cout << "\tBELOW (" << max->position() << ") -- (" << mid->position() << ")" << std::endl;
        // estoy abajo de max -- mid
        edgeMaxMid->treeNodeBelow(node);
        if (edgeMaxMid->treeNodeAbove()) {
            node->connect(edgeMaxMid->treeNodeAbove());
        }
    }

    auto edgeMidMin = mid->addTriangulationEdge(min);

    if((midIsLeft && min->position().getX() > mid->position().getX())
        || (!midIsLeft && min->position().getX() <= mid->position().getX())) {
//        std::cout << "\tABOVE (" << mid->position() << ") -- (" << min->position() << ")" << std::endl;
        // estoy arriba de mid -- min
        edgeMidMin->treeNodeAbove(node);
        if (edgeMidMin->treeNodeBelow()) {
            node->connect(edgeMidMin->treeNodeBelow());
        }
    } else {
//        std::cout << "\tBELOW (" << mid->position() << ") -- (" << min->position() << ")" << std::endl;
        // estoy abajo de mid -- min
        edgeMidMin->treeNodeBelow(node);
        if (edgeMidMin->treeNodeAbove()) {
            node->connect(edgeMidMin->treeNodeAbove());
        }
    }
}

TreeNodePtr TreeNode::create(Vertex::Ptr v1, Vertex::Ptr v2, Vertex::Ptr v3)
{
    auto node = TreeNodePtr(new TreeNode(v1, v2, v3));

    /*std::cout << "TREE_NODE (" << node->_id << "): (" << v1->position() << ") -- ("
              << v2->position() << ") -- ("
              << v3->position() << ")" << std::endl;*/

    if(*v1 > *v2) {
        if(*v1 > *v3) {
            // v1 max
            if(*v2 > *v3) {
                // v1 -> v2 -> v3
                connectTree(node, v1, v2, v3);
            } else {
                // v1 -> v3 -> v2
                connectTree(node, v1, v3, v2);
            }
        } else {
            // v3 -> v1 -> v2
            connectTree(node, v3, v1, v2);
        }
    } else {
        if(*v2 > *v3) {
            // v2 max
            if(*v1 > *v3) {
                // v2 -> v1 -> v3
                connectTree(node, v2, v1, v3);
            } else {
                // v2 -> v3 -> v1
                connectTree(node, v2, v3, v1);
            }
        } else {
            // v3 -> v2 -> v1
            connectTree(node, v3, v2, v1);
        }
    }

    return node;
}

VertexPtr TreeNode::vertexA() const {
    return this->_v1;
}

VertexPtr TreeNode::vertexB() const {
    return this->_v2;
}

VertexPtr TreeNode::vertexC() const {
    return this->_v3;
}

bool TreeNode::visited() const {
    return this->_visited;
}

void TreeNode::visited(bool visited) {
    this->_visited = visited;
}

void TreeNode::_addNeighbour(TreeNodePtr neighbour) {
    this->_neighbours.add(std::move(neighbour));
}

bool TreeNode::isNeighbour(TreeNodePtr const& other) const {
    auto const& mVertexA = *this->vertexA();
    auto const& mVertexB = *this->vertexB();
    auto const& mVertexC = *this->vertexC();

    auto const& oVertexA = *other->vertexA();
    auto const& oVertexB = *other->vertexB();
    auto const& oVertexC = *other->vertexC();

    const bool vAShared = mVertexA == oVertexA || mVertexA == oVertexB || mVertexA == oVertexC;
    const bool vBShared = mVertexB == oVertexA || mVertexB == oVertexB || mVertexB == oVertexC;
    const bool vCShared = mVertexC == oVertexA || mVertexC == oVertexB || mVertexC == oVertexC;

    unsigned short cantShared = 0;
    if(vAShared) ++cantShared;
    if(vBShared) ++cantShared;
    if(vCShared) ++cantShared;

    return cantShared >= 2;
}

void TreeNode::connect(TreeNodePtr neighbour) {
    if(!this->isNeighbour(neighbour)) {
        // Los nodos no son vecinos
//        std::cout << "\tSKIPPED TREE_CONNECT: " << this->id() << " <--> " << neighbour->id() << std::endl;
        return;
    }

//    std::cout << "\tTREE_CONNECT: " << this->id() << " <--> " << neighbour->id() << std::endl;

    neighbour->_addNeighbour(this->shared_from_this());
    this->_addNeighbour(std::move(neighbour));
}

List<TreeNodePtr> const &TreeNode::neighbours() const {
    return this->_neighbours;
}

std::size_t TreeNode::getNextID() {
    static std::size_t nextID = 0;
    return nextID++;
}

using Color = Vertex::Color;

struct ColorInfo {
    List<Point> & colorA, & colorB, & colorC;

    void addColorA(VertexPtr const& vertex) {
        vertex->color(Color::COLOR_A);
        this->colorA.add(vertex->position());
    }
    void addColorB(VertexPtr const& vertex) {
        vertex->color(Color::COLOR_B);
        this->colorB.add(vertex->position());
    }
    void addColorC(VertexPtr const& vertex) {
        vertex->color(Color::COLOR_C);
        this->colorC.add(vertex->position());
    }
};

void colorizeHelper(ColorInfo & ci, VertexPtr const& v1, VertexPtr const& v2, VertexPtr const& v3) {
    ci.addColorA(v1);
    ci.addColorB(v2);
    ci.addColorC(v3);
}

void colorizeHelper(ColorInfo & ci, VertexPtr const& v1, VertexPtr const& v2, Vertex::Color c1) {
    switch (c1) {
        case Color::COLOR_A: {
            ci.addColorB(v1);
            ci.addColorC(v2);
            break;
        }
        case Color::COLOR_B: {
            ci.addColorA(v1);
            ci.addColorC(v2);
            break;
        }
        case Color::COLOR_C: {
            ci.addColorA(v1);
            ci.addColorB(v2);
            break;
        }
        case Vertex::UNDEFINED:
            break;
    }
}

void colorizeHelper(ColorInfo & ci, VertexPtr const& v1, Vertex::Color c1, Vertex::Color c2) {
    switch (c1) {
        case Vertex::COLOR_A: {
            switch (c2) {
                case Vertex::COLOR_B: {
                    ci.addColorC(v1);
                    break;
                }
                case Vertex::COLOR_C: {
                    ci.addColorB(v1);
                    break;
                }

                case Vertex::COLOR_A:
                case Vertex::UNDEFINED:
                    break;
            }
            break;
        }
        case Vertex::COLOR_B: {
            switch (c2) {
                case Vertex::COLOR_A: {
                    ci.addColorC(v1);
                    break;
                }
                case Vertex::COLOR_C: {
                    ci.addColorA(v1);
                    break;
                }

                case Vertex::COLOR_B:
                case Vertex::UNDEFINED:
                    break;
            }
            break;
        }
        case Vertex::COLOR_C: {
            switch (c2) {
                case Vertex::COLOR_A: {
                    ci.addColorB(v1);
                    break;
                }
                case Vertex::COLOR_B: {
                    ci.addColorA(v1);
                    break;
                }

                case Vertex::COLOR_C:
                case Vertex::UNDEFINED:
                    break;
            }

            break;
        }

        case Vertex::UNDEFINED:
            break;
    }
}

void TreeNode::colorize(List<Point> & colorA, List<Point> & colorB, List<Point> & colorC) {
    ColorInfo ci{colorA, colorB, colorC};

    if(this->vertexA()->color() == Color::UNDEFINED) {
        if(this->vertexB()->color() == Color::UNDEFINED) {
            if(this->vertexC()->color() == Color::UNDEFINED) {
                colorizeHelper(
                        ci,
                        this->vertexA(),
                        this->vertexB(),
                        this->vertexC()
                );
            } else {
                colorizeHelper(
                        ci,
                        this->vertexA(),
                        this->vertexB(),
                        this->vertexC()->color()
                );
            }
        } else {
            if(this->vertexC()->color() == Color::UNDEFINED) {
                colorizeHelper(
                        ci,
                        this->vertexA(),
                        this->vertexC(),
                        this->vertexB()->color()
                );
            } else {
                colorizeHelper(
                        ci,
                        this->vertexA(),
                        this->vertexB()->color(),
                        this->vertexC()->color()
                );
            }
        }
    } else {
        if(this->vertexB()->color() == Color::UNDEFINED) {
            if(this->vertexC()->color() == Color::UNDEFINED) {
                colorizeHelper(
                        ci,
                        this->vertexB(),
                        this->vertexC(),
                        this->vertexA()->color()
                );
            } else {
                colorizeHelper(
                        ci,
                        this->vertexB(),
                        this->vertexA()->color(),
                        this->vertexC()->color()
                );
            }
        } else if(this->vertexC()->color() == Color::UNDEFINED) {
            colorizeHelper(
                    ci,
                    this->vertexC(),
                    this->vertexA()->color(),
                    this->vertexB()->color()
            );
        }
    }
}

bool TreeNode::operator==(const TreeNode &rhs) const {
    return this->_id == rhs._id;
}

bool TreeNode::operator!=(const TreeNode &rhs) const {
    return !(rhs == *this);
}
