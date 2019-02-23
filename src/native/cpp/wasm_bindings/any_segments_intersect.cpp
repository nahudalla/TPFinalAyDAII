#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <wasm_bindings/data_input.hpp>
#include <wasm_bindings/utils.hpp>
#include <wasm_bindings/errors.hpp>

#include <algor/AnySegmentsIntersect.hpp>

using namespace wasm_bindings;
using namespace algor;
using namespace emscripten;

val EMSCRIPTEN_KEEPALIVE run_any_segments_intersect() {
    if(!verify_list_of_segments_input()) {
        return generateListOfSegmentsInputNotSetError();
    }

    AnySegmentsIntersect asi(*(inputData.asListOfSegments));

    if(!asi.canRun()) {
        return generateInvalidInputError();
    }

    auto res = asi.run();

    if(res.has_value()) return val(*res);

    return val::undefined();
}

EMSCRIPTEN_BINDINGS(algorithm_any_segments_intersect) {
        function("run_any_segments_intersect", &run_any_segments_intersect, allow_raw_pointers());
}
