#include <optional>
#include <cmath>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <algor/List.hpp>
#include <algor/Point.hpp>
#include <algor/Segment.hpp>

#include <wasm_bindings/data_input.hpp>
#include <wasm_bindings/errors.hpp>
#include <iostream>

using namespace emscripten;
using namespace wasm_bindings;
using namespace algor;

namespace wasm_bindings {
    EMSCRIPTEN_KEEPALIVE InputData inputData;
    EMSCRIPTEN_KEEPALIVE emscripten::val CONSTANTS = val::undefined();
}

val EMSCRIPTEN_KEEPALIVE wasm_bindings::set_constants(emscripten::val constants) {
    if(constants.typeOf().as<std::string>() != "object") {
        return generateInvalidConstantsError();
    }

    auto PROGRESS_IDS = constants[val("PROGRESS_IDS")];

    if(PROGRESS_IDS.typeOf().as<std::string>() != "object") {
        return generateInvalidConstantsError();
    }

    CONSTANTS = std::move(constants);

    return val::undefined();
}

void EMSCRIPTEN_KEEPALIVE wasm_bindings::set_raw_input(long coords, unsigned int length) {
    auto coords_ptr = reinterpret_cast<int *>(coords);

    std::free(inputData.coords);
    inputData.coords = coords_ptr;
    inputData.length = length;
}

val EMSCRIPTEN_KEEPALIVE wasm_bindings::prepare_list_of_points_input() {
    if(inputData.coords == nullptr) return generateRawInputNotSetError();

    List<Point> list;

    for(std::size_t i = 0; i < inputData.length-1; i += 2) {
        list.add(Point(inputData.coords[i], inputData.coords[i+1]));
    }

    inputData.asListOfPoints = std::move(list);

    return val::undefined();
}

val EMSCRIPTEN_KEEPALIVE wasm_bindings::prepare_array_of_points_input() {
    if(inputData.coords == nullptr && inputData.asListOfPoints == std::nullopt) {
        return generateRawInputNotSetError();
    }

    if(inputData.coords != nullptr) {
        Array<Point> arr(inputData.length/2);

        for(std::size_t i = 0; i < arr.size(); ++i) {
            arr[i] = Point(inputData.coords[i*2], inputData.coords[i*2+1]);
        }

        inputData.asArrayOfPoints = std::move(arr);
    } else {
        inputData.asArrayOfPoints = Array<Point>(*(inputData.asListOfPoints));
    }

    return val::undefined();
}

val EMSCRIPTEN_KEEPALIVE wasm_bindings::prepare_list_of_segments_input() {
    if(inputData.coords == nullptr) return generateRawInputNotSetError();

    if(inputData.length < 4) return generateInvalidInputError();

    List<Segment> list;

    for(std::size_t i = 0; i < inputData.length-3; i += 4) {
        list.add(Segment(
                Point(inputData.coords[i], inputData.coords[i+1]),
                Point(inputData.coords[i+2], inputData.coords[i+3])
        ));
    }

    inputData.asListOfSegments = std::move(list);

    return val::undefined();
}

void EMSCRIPTEN_KEEPALIVE wasm_bindings::clear_raw_input() {
    std::free(inputData.coords);
    inputData.coords = nullptr;
    inputData.length = 0;
}

void EMSCRIPTEN_KEEPALIVE wasm_bindings::clear_list_of_points_input() {
    inputData.asListOfPoints = std::nullopt;
}

void EMSCRIPTEN_KEEPALIVE wasm_bindings::clear_list_of_segments_input() {
    inputData.asListOfSegments = std::nullopt;
}

void EMSCRIPTEN_KEEPALIVE wasm_bindings::clear_array_of_points_input() {
    inputData.asArrayOfPoints = std::nullopt;
}

EMSCRIPTEN_BINDINGS(data_input) {
        function("set_raw_input", &set_raw_input, allow_raw_pointers());
        function("set_constants", &set_constants);
        function("prepare_list_of_points_input", &prepare_list_of_points_input);
        function("prepare_array_of_points_input", &prepare_array_of_points_input);
        function("prepare_list_of_segments_input", &prepare_list_of_segments_input);
        function("clear_raw_input", &clear_raw_input);
        function("clear_list_of_points_input", &clear_list_of_points_input);
        function("clear_list_of_segments_input", &clear_list_of_segments_input);
        function("clear_array_of_points_input", &clear_array_of_points_input);
}
