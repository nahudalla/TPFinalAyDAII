#include <utility>

#include <algor/Array.hpp>
#include <algor/Point.hpp>
#include <algor/RBTree.hpp>
#include <algor/Stack.hpp>
#include <algor/Segment.hpp>

#include <algor/SimplePolygon_impl/Vertex.hpp>
#include <algor/SimplePolygon_impl/TreeNode.hpp>

#include <algor/SimplePolygon.hpp>

using namespace algor::__detail_SimplePolygon;
using namespace algor;

bool isClockwise(Array<Point> const& vertices) {
    const auto sz = vertices.size();

    std::size_t  max = 0, i = 1;
    for(; i < sz; ++i) {
        if(vertices[i].getY() > vertices[max].getY()
            || (vertices[i].getY() == vertices[max].getY()
                && vertices[i].getX() < vertices[max].getX())
        ) { max = i; }
    }

    return Segment(vertices[max == 0 ? sz-1 : max-1], vertices[max])
            .turnsRight(Segment(vertices[max], vertices[(max+1)%sz]));
}

SimplePolygon::SimplePolygon(Array<Point> const& vertices)
{
    // TODO: test

    if(vertices.size() < 3) {
        return;
    }

    this->_vertices = Array<VertexPtr>(vertices.size());

    // Supongo que los vértices están en sentido antihorario,
    // recorro el arreglo de principio a fin
    std::size_t begin = 0, end = vertices.size()-1;
    short step = 1;

    // Si los vértices están en sentido horario, invierto el recorrido del arreglo
    if(isClockwise(vertices)) {
//        std::cout << "CLOCKWISE" << std::endl;
        begin = end;
        end = 0;
        step = -1;
    } /*else std::cout << "COUNTER" << std::endl;*/

    // Recorro el arreglo de vértices para armar la lista doblemente vinculada de aristas
    Vertex::Ptr current = Vertex::create(vertices[begin]);
    std::size_t vertexIndex = 0;
    for(auto i = begin; i != end; i += step) {
        auto next = Vertex::create(vertices[i+step]);

        current->next(next);

        (*this->_vertices)[vertexIndex++] = std::move(current);

        current = std::move(next);
    }

    // Vinculo el primer y el último vértice y agrego éste a la lista de vértices
    Vertex::Ptr & first = (*this->_vertices)[0];

    current->next(first);

    (*this->_vertices)[vertexIndex] = std::move(current);
}

Comparator<VertexPtr>::Result edgeBSTComparator(VertexPtr const& e1, VertexPtr const& e2) {
    // TODO: test

    typedef Comparator<VertexPtr>::Result Result;

    if(e1->position() == e2->position()) return Result::EQUAL;

    if(e1->edgeSegment().getTopmostEndpoint().getX()
        < e2->edgeSegment().getTopmostEndpoint().getX())
    { return Result::LESS; }
    else { return Result::GREATER; }
}

std::pair<List<Segment>, Vertex::Ptr> SimplePolygon::generateMonotonePartitions() {
    // TODO: test

    List<Segment> segments;

    if(!this->_vertices) {
        return std::make_pair(segments, nullptr);
    }

    auto sortedVertices = merge_sort(*this->_vertices, 0, this->_vertices->size());

    auto searchTree = RBTree<VertexPtr>(Comparator<VertexPtr>(edgeBSTComparator));

    auto current = sortedVertices;
    for(std::size_t i = 0; current; ++i, current = current->nextBelow()) {
        const auto leftOfBSTComparator = (UnaryComparator<VertexPtr>::ComparatorFunction)
                [&current](VertexPtr const& other) {
                    typedef UnaryComparator<VertexPtr>::Result Result;

                    if(*other == *current) return Result::EQUAL;
                    else if(other->position().getX() < current->position().getX()
                            || (other->position().getX() == current->position().getX()
                                && other->position().getY() > current->position().getY())
                    ) { return Result::LESS; }
                    else return Result::GREATER;
                };

        switch (current->type()) {
            case Vertex::START: {
                current->helper(current);
                searchTree.insert(current);
                break;
            }
            case Vertex::END: {
                auto prev = current->previous()->to();
                auto prevHelper = prev->helper();
                if(prevHelper->type() == Vertex::MERGE) {
                    segments.add(Segment(current->position(), prevHelper->position()));
                    current->addDiagonal(std::move(prevHelper));
                }
                searchTree.remove(prev);
                break;
            }
            case Vertex::SPLIT: {
                auto edgeLeftOfCurrent = searchTree.findMaxLessThan(leftOfBSTComparator);
                segments.add(Segment(current->position(), (*edgeLeftOfCurrent)->helper()->position()));
                current->addDiagonal((*edgeLeftOfCurrent)->helper());
                (*edgeLeftOfCurrent)->helper(current);
                current->helper(current);
                searchTree.insert(current);
                break;
            }
            case Vertex::MERGE: {
                auto prev = current->previous()->to();
                auto prevHelper = prev->helper();
                if(prevHelper->type() == Vertex::MERGE) {
                    segments.add(Segment(current->position(), prevHelper->position()));
                    current->addDiagonal(std::move(prevHelper));
                }
                searchTree.remove(prev);
                auto edgeLeftOfCurrent = searchTree.findMaxLessThan(leftOfBSTComparator);
                auto eHelper = (*edgeLeftOfCurrent)->helper();
                if(eHelper->type() == Vertex::MERGE) {
                    segments.add(Segment(current->position(), eHelper->position()));
                    current->addDiagonal(eHelper);
                }
                (*edgeLeftOfCurrent)->helper(current);
                break;
            }
            case Vertex::REGULAR_LEFT: {
                auto prev = current->previous()->to();
                auto prevHelper = prev->helper();
                if(prevHelper->type() == Vertex::MERGE) {
                    segments.add(Segment(current->position(), prevHelper->position()));
                    current->addDiagonal(prevHelper);
                }
                searchTree.remove(prev);
                current->helper(current);
                searchTree.insert(current);
                break;
            }
            case Vertex::REGULAR_RIGHT: {
                auto edgeLeftOfCurrent = searchTree.findMaxLessThan(leftOfBSTComparator);
                auto eHelper = (*edgeLeftOfCurrent)->helper();
                if(eHelper->type() == Vertex::MERGE) {
                    segments.add(Segment(current->position(), eHelper->position()));
                    current->addDiagonal(eHelper);
                }
                (*edgeLeftOfCurrent)->helper(current);
                break;
            }

            // Si se llega a este caso, los datos de entrada no eran válidos o hubo un error
            case Vertex::UNKNOWN:
            default:
                return std::make_pair(std::move(segments), VertexPtr());
        }
    }

    return std::make_pair(std::move(segments), std::move(sortedVertices));
}

List<Segment> SimplePolygon::monotonePartitions() {
    // TODO: test

    return std::move(this->generateMonotonePartitions().first);
}

std::pair<List<Segment>, TreeNodePtr> SimplePolygon::generateTriangulation() {
    auto partitions = this->generateMonotonePartitions();

    TreeNodePtr firstTreeNode;

    // Todas las diagonales de la partición en monótono, también son parte de la triangulación
    auto triangulation = std::move(partitions.first);
    auto current_global = std::move(partitions.second);

    if(!current_global) {
        return std::make_pair(std::move(triangulation), std::move(firstTreeNode));
    }

    enum CHAIN_SIDE {LEFT, RIGHT, TOP};

    // Creo un elemento "fantasma" arriba del elemento más alto.
    // Como todos los elementos son eliminados de la lista de vértices ordenada,
    // el siguiente del elemento "fantasma" siempre va a quedar actualizado
    // apuntando al elemento más alto global actual. Cuando no haya más elementos
    // que procesar, el siguiente del fantasma va a ser "nullptr" ya que será el
    // último elemento que quede en la lista

    auto prevToGlobalTop = Vertex::create(Point());
    prevToGlobalTop->nextBelow(current_global);
    current_global->previousAbove(prevToGlobalTop);

    for(; current_global; current_global = prevToGlobalTop->nextBelow()) {
//        std::cout << "Global: (" << current_global->position() << ")" << std::endl;

        auto CI_CD = current_global->visitStart();

        VertexPtr CI_prev, CD_prev;
        auto CI = std::move(CI_CD.first);
        auto CD = std::move(CI_CD.second);

        if(CI && CD) {
//            std::cout << "CI: (" << CI->position() << "); CD: (" << CD->position() << ")" << std::endl;

            Stack<std::pair<VertexPtr, CHAIN_SIDE>> stack;

            stack.push(std::make_pair(current_global, TOP));

            // Agrego el siguiente del subpolígono a la pila
            if (*CI > *CD) {
                stack.push(std::make_pair(CI, LEFT));
//                std::cout << "ADDED_STACK: (" << CI->position() << ")" << std::endl;
                CI_prev = CI;
                CI = CI->visitLeftChain();
                CD_prev = current_global;
            } else {
                stack.push(std::make_pair(CD, RIGHT));
//                std::cout << "ADDED_STACK: (" << CD->position() << ")" << std::endl;
                CD_prev = CD;
                CD = CD->visitRightChain();
                CI_prev = current_global;
            }

//            std::cout << "CI: (" << CI->position() << "); CD: (" << CD->position() << ")" << std::endl;

            while(CI && CD && *CI != *CD) {

                // Elijo el siguiente del subpolígono
                VertexPtr current;
                CHAIN_SIDE current_side;

                if(*CI > *CD) {
                    current = CI;
//                    std::cout << "CURRENT: (" << current->position() << ")" << std::endl;
                    current_side = LEFT;
                    auto CI_next = CI->visitLeftChain();
                    CI_prev->attemptRemoveFromOrderedList();
                    CI_prev = CI;
                    CI = CI_next;
                } else {
                    current = CD;
//                    std::cout << "CURRENT: (" << current->position() << ")" << std::endl;
                    current_side = RIGHT;
                    auto CD_next = CD->visitRightChain();
                    CD_prev->attemptRemoveFromOrderedList();
                    CD_prev = CD;
                    CD = CD_next;
                }

                if(stack.top().second != current_side) {
                    // Si el vértice actual y el tope de la pila no están en la misma cadena

                    /* Pop all vertices from S.
                       Insert into D a diagonal from u j to each popped vertex, except the last one.
                       Push u j−1 and u j onto S
                   */

                    auto first = stack.pop();
                    auto prev = first;
                    auto connect = prev;
                    while(!stack.isEmpty()) {
//                        std::cout << "TRIANGULATION1: (" << current->position() << ") -- (" << connect.first->position() << ")" << std::endl;

                        triangulation.add(Segment(current->position(), connect.first->position()));

                        TreeNode::create(connect.first, stack.top().first, current);

                        // TODO: REMOVE
//                        current->addTriangulationDiagonal(connect.first);

                        prev = connect;
                        connect = stack.pop();
                    }
                    stack.push(first);
//                    std::cout << "ADDED_STACK: (" << current->position() << ")" << std::endl;
                    stack.push(std::make_pair(current, current_side));
                } else {
                    // Si el vértice en el tope de la pila y el actual están en la misma cadena
                    /* Pop one vertex from S.
                       Pop the other vertices from S as long as the diagonals from
                       u j to them are inside P. Insert these diagonals into D. Push
                       the last vertex that has been popped back onto S.
                       Push u j onto S.
                     */

                    auto prev = stack.pop();
                    while(!stack.isEmpty())
                    {
                        auto curr = stack.top();

                        bool turnsLeft = Segment(current->position(), prev.first->position())
                                .turnsLeft(Segment(prev.first->position(), curr.first->position()));
                        bool turnsRight = Segment(current->position(), prev.first->position())
                                .turnsRight(Segment(prev.first->position(), curr.first->position()));

                        if((current_side == LEFT && turnsRight) || (current_side == RIGHT && turnsLeft))
                        {
//                            std::cout << "TRIANGULATION2: (" << current->position() << ") -- (" << curr.first->position() << ")" << std::endl;
                            triangulation.add(Segment(current->position(), curr.first->position()));

                            TreeNode::create(prev.first, curr.first, current);

                            // TODO: REMOVE
//                            current->addTriangulationDiagonal(curr.first);

                            prev = stack.pop();
                        } else break;
                    }

                    stack.push(prev);
//                    std::cout << "ADDED_STACK: (" << current->position() << ")" << std::endl;
                    stack.push(std::make_pair(current, current_side));
                }
            }

            if(CI && CD && *CI == *CD) {
//                std::cout << "CURRENT: (" << CI->position()<< ")" << std::endl;

                // Add diagonals from u n to all stack vertices except the first and the last one.
                auto prev = stack.pop();
                while(!stack.isEmpty()) {
                    auto curr = stack.pop();
                    if(stack.isEmpty()) {
                        auto treeNode = TreeNode::create(prev.first, curr.first, CI);

                        if(!firstTreeNode) firstTreeNode = std::move(treeNode);

                        break;
                    }
//                    std::cout << "TRIANGULATION3: (" << CI->position() << ") -- (" << curr.first->position() << ")" << std::endl;
                    triangulation.add(Segment(CI->position(), curr.first->position()));

                    TreeNode::create(prev.first, curr.first, CI);

                    // TODO: REMOVE
//                    CI->addTriangulationDiagonal(curr.first);

                    prev = curr;
                }

                CI->visited(true);
                CI_prev->attemptRemoveFromOrderedList();
                CD_prev->attemptRemoveFromOrderedList();
                CI->attemptRemoveFromOrderedList();
            }
        }
        // Si no hay cadena izquierda o derecha, hubo un error
        if(!CI || !CD) {
//            std::cout << "EMPTY CHAINS" << std::endl;

            triangulation.clear();
            firstTreeNode.reset();
            break;
        }
    }

    return std::make_pair(std::move(triangulation), std::move(firstTreeNode));
}

List<Segment> SimplePolygon::triangulate()
{
    return std::move(this->generateTriangulation().first);
}

std::tuple<List<Point>, List<Point>, List<Point>> SimplePolygon::threeColoring() {
    List<Point> colorA, colorB, colorC;

    if(this->_vertices) {
        auto treeNode = std::move(this->generateTriangulation().second);

        if(!treeNode) {
            // Hubo un error
            return std::make_tuple(std::move(colorA), std::move(colorB), std::move(colorC));
        }

        // Recorro en DFS el grafo dual de la triangulación del polígono
        // (que es un árbol) y coloreo desde la raíz hacia las hojas.
        // La raíz del árbol puede ser cualquier nodo
        Stack<TreeNodePtr> stack;
        stack.push(treeNode);

        while(!stack.isEmpty()) {
            auto current = stack.pop();
            current->visited(true);
            current->colorize(colorA, colorB, colorC);

            const auto &neighbours = current->neighbours();
            for (auto it = neighbours.cbegin(); it != neighbours.cend(); ++it) {
                if ((*it)->visited()) continue;

                stack.push(*it);
            }
        }
    }

    return std::make_tuple(std::move(colorA), std::move(colorB), std::move(colorC));
}
