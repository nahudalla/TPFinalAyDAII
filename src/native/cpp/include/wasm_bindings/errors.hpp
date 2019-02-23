#ifndef TPFINALAYDAII_WASM_BINDINGS_ERRORS_HPP
#define TPFINALAYDAII_WASM_BINDINGS_ERRORS_HPP

#include <emscripten/val.h>

namespace wasm_bindings {
    emscripten::val generateError(std::string message);
    emscripten::val generateInvalidConstantsError();
    emscripten::val generateConstantsNotDefinedError();
    emscripten::val generateRawInputNotSetError();
    emscripten::val generateInvalidProgressCallback();
    emscripten::val generateListOfPointsInputNotSetError();
    emscripten::val generateListOfSegmentsInputNotSetError();
    emscripten::val generateArrayOfPointsInputNotSetError();
    emscripten::val generateInvalidInputError();
}

#endif //TPFINALAYDAII_WASM_BINDINGS_ERRORS_HPP
