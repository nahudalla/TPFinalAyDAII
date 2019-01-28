#include <string>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

val EMSCRIPTEN_KEEPALIVE run_any_segments_intersect() {
    return val::undefined();
}

EMSCRIPTEN_BINDINGS(algorithm_exports) {
    function("run_any_segments_intersect", &run_any_segments_intersect, allow_raw_pointers());
}
