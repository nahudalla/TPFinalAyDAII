#ifndef TPFINALAYDAII_ALGOR_SIMPLEPOLYGON_HPP
#define TPFINALAYDAII_ALGOR_SIMPLEPOLYGON_HPP

#include <memory>

#include <optional>
#include <utility>

#include <algor/Array.hpp>
#include <algor/List.hpp>
#include <algor/Point.hpp>
#include <algor/Segment.hpp>
#include <algor/SimplePolygon_impl/TreeNode.hpp>

namespace algor {
    namespace __detail_SimplePolygon {
        class Vertex;
    }

    class SimplePolygon {
        typedef std::shared_ptr<__detail_SimplePolygon::Vertex> VertexPtr;

        std::optional<Array<VertexPtr>> _vertices;

        std::pair<List<Segment>, VertexPtr> generateMonotonePartitions();
        std::pair<List<Segment>, __detail_SimplePolygon::TreeNodePtr> generateTriangulation();
    public:
        explicit SimplePolygon(Array<Point> const&);

        List<Segment> monotonePartitions();
        List<Segment> triangulate();
        std::tuple<List<Point>, List<Point>, List<Point>> threeColoring();
    };
}

#endif //TPFINALAYDAII_ALGOR_SIMPLEPOLYGON_HPP
