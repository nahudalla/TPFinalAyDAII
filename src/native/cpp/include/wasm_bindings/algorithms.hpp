#ifndef TPFINALAYDAII_WASM_BINDINGS_GRAHAM_SCAN_HPP
#define TPFINALAYDAII_WASM_BINDINGS_GRAHAM_SCAN_HPP

#include <emscripten.h>
#include <emscripten/val.h>

namespace wasm_bindings {
    emscripten::val EMSCRIPTEN_KEEPALIVE run_graham_scan();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_jarvis_march();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_closest_points();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_any_segments_intersect();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_partition_simple_polygon_to_monotone();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_triangulate_simple_polygon();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_art_gallery_problem_on_simple_polygon();
}

#endif //TPFINALAYDAII_WASM_BINDINGS_GRAHAM_SCAN_HPP
