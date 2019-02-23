#ifndef TPFINALAYDAII_WASM_BINDINGS_GRAHAM_SCAN_HPP
#define TPFINALAYDAII_WASM_BINDINGS_GRAHAM_SCAN_HPP

#include <emscripten.h>
#include <emscripten/val.h>

namespace wasm_bindings {
    emscripten::val EMSCRIPTEN_KEEPALIVE run_graham_scan();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_jarvis_march();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_closest_points();
    emscripten::val EMSCRIPTEN_KEEPALIVE run_any_segments_intersect();
}

#endif //TPFINALAYDAII_WASM_BINDINGS_GRAHAM_SCAN_HPP
