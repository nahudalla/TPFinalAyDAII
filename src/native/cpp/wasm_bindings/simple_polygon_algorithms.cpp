#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <wasm_bindings/algorithms.hpp>
#include <wasm_bindings/data_input.hpp>
#include <wasm_bindings/utils.hpp>
#include <wasm_bindings/errors.hpp>

#include <algor/SimplePolygon.hpp>
#include <algor/Segment.hpp>
#include <algor/List.hpp>

using namespace emscripten;
using namespace wasm_bindings;
using namespace algor;

val generateResponseFromListOfSegments(List<Segment> const&);

val EMSCRIPTEN_KEEPALIVE wasm_bindings::run_partition_simple_polygon_to_monotone() {
    if(!verify_array_of_points_input()) {
        return generateArrayOfPointsInputNotSetError();
    }

    SimplePolygon sp(*(inputData.asArrayOfPoints));

    return generateResponseFromListOfSegments(sp.monotonePartitions());
}

val EMSCRIPTEN_KEEPALIVE wasm_bindings::run_triangulate_simple_polygon() {
    if(!verify_array_of_points_input()) {
        return generateArrayOfPointsInputNotSetError();
    }

    SimplePolygon sp(*(inputData.asArrayOfPoints));

    return generateResponseFromListOfSegments(sp.triangulate());
}

val EMSCRIPTEN_KEEPALIVE wasm_bindings::run_art_gallery_problem_on_simple_polygon() {
    if(!verify_array_of_points_input()) {
        return generateArrayOfPointsInputNotSetError();
    }

    SimplePolygon sp(*(inputData.asArrayOfPoints));

    auto tuple = sp.threeColoring();

    const auto sz1 = std::get<0>(tuple).length();
    const auto sz2 = std::get<1>(tuple).length();
    const auto sz3 = std::get<2>(tuple).length();

    if(sz1 > 0 || sz2 > 0 || sz3 > 0) {
        const auto sz = (sz1 + sz2 + sz3)*2;
        int * data = new int[sz];

        auto response = val::object();

        std::size_t i = 0;
        for(auto it = std::get<0>(tuple).cbegin(); it != std::get<0>(tuple).cend(); ++it) {
            data[i++] = it->getX();
            data[i++] = it->getY();
        }
        for(auto it = std::get<1>(tuple).cbegin(); it != std::get<1>(tuple).cend(); ++it) {
            data[i++] = it->getX();
            data[i++] = it->getY();
        }
        for(auto it = std::get<2>(tuple).cbegin(); it != std::get<2>(tuple).cend(); ++it) {
            data[i++] = it->getX();
            data[i++] = it->getY();
        }

        response.set(val("data"), val(reinterpret_cast<long>(data)));
        response.set(val("length"), val(sz));
        response.set(val("colorASize"), val(sz1));
        response.set(val("colorBSize"), val(sz2));
        response.set(val("colorCSize"), val(sz3));

        return std::move(response);
    }

    return val::null();
}

EMSCRIPTEN_BINDINGS(algorithms_simple_polygon) {
    function("run_partition_simple_polygon_to_monotone", &run_partition_simple_polygon_to_monotone);
    function("run_triangulate_simple_polygon", &run_triangulate_simple_polygon);
    function("run_art_gallery_problem_on_simple_polygon", &run_art_gallery_problem_on_simple_polygon);
}

val generateResponseFromListOfSegments(List<Segment> const& list) {
    auto sz = list.length()*4;
    if(sz > 0) {
        int * data = new int[sz];

        auto response = val::object();

        std::size_t i = 0;
        for(auto it = list.cbegin(); it != list.cend(); ++it) {
            Segment const& segment = *it;

            data[i++] = segment.getFrom().getX();
            data[i++] = segment.getFrom().getY();
            data[i++] = segment.getTo().getX();
            data[i++] = segment.getTo().getY();
        }

        response.set(val("data"), val(reinterpret_cast<long>(data)));
        response.set(val("length"), val(sz));

        return std::move(response);
    }

    return val::null();
}
