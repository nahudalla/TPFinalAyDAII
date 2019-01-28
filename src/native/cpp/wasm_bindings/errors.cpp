#include <string>

#include <emscripten/val.h>

#include <wasm_bindings/errors.hpp>

using namespace emscripten;
using namespace wasm_bindings;

val wasm_bindings::generateError(std::string message) {
    return val::global("Error").new_(val(std::move(message)));
}

val wasm_bindings::generateInvalidConstantsError() {
    return generateError("Invalid constants object.");
}

val wasm_bindings::generateConstantsNotDefinedError() {
    return generateError("Constants not initialized in native module.");
}

val wasm_bindings::generateRawInputNotSetError() {
    return generateError("Raw input not set.");
}

val wasm_bindings::generateInvalidProgressCallback() {
    return generateError("Invalid progress callback, must be a function.");
}

emscripten::val wasm_bindings::generateListOfPointsInputNotSetError() {
    return generateError("List of points input not set.");
}

emscripten::val wasm_bindings::generateArrayOfPointsInputNotSetError() {
    return generateError("Array of points input not set.");
}

emscripten::val wasm_bindings::generateInvalidInputError() {
    return generateError("Invalid input data.");
}
