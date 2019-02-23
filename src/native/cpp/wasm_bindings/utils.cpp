#include <string>

#include <wasm_bindings/utils.hpp>
#include <wasm_bindings/data_input.hpp>

#include <iostream>

using namespace wasm_bindings;

bool wasm_bindings::verify_progress_callback(emscripten::val const &progress) {
    return progress.typeOf().as<std::string>() != "function";
}

bool wasm_bindings::verify_list_of_points_input() {
    return inputData.asListOfPoints.has_value();
}

bool wasm_bindings::verify_array_of_points_input() {
    return inputData.asArrayOfPoints.has_value();
}

bool wasm_bindings::verify_list_of_segments_input() {
    return inputData.asListOfSegments.has_value();
}
