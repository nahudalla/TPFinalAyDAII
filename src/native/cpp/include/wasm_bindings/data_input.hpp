#ifndef TPFINALAYDAII_WASM_BINDINGS_DATA_INPUT_HPP
#define TPFINALAYDAII_WASM_BINDINGS_DATA_INPUT_HPP

#include <optional>

#include <emscripten.h>
#include <emscripten/val.h>

#include <algor/Array.hpp>
#include <algor/List.hpp>
#include <algor/Point.hpp>
#include <algor/Segment.hpp>

namespace wasm_bindings {
    struct InputData {
        unsigned int length = 0;
        std::optional<algor::List<algor::Point>> asListOfPoints = std::nullopt;
        std::optional<algor::Array<algor::Point>> asArrayOfPoints = std::nullopt;
        std::optional<algor::List<algor::Segment>> asListOfSegments = std::nullopt;
        int * coords = nullptr;
    };

    emscripten::val EMSCRIPTEN_KEEPALIVE set_constants(emscripten::val constants);

    void EMSCRIPTEN_KEEPALIVE set_raw_input(long coords, unsigned int length);
    emscripten::val EMSCRIPTEN_KEEPALIVE prepare_list_of_points_input();
    emscripten::val EMSCRIPTEN_KEEPALIVE prepare_array_of_points_input();
    emscripten::val EMSCRIPTEN_KEEPALIVE prepare_list_of_segments_input();

    void EMSCRIPTEN_KEEPALIVE clear_raw_input();
    void EMSCRIPTEN_KEEPALIVE clear_list_of_points_input();
    void EMSCRIPTEN_KEEPALIVE clear_list_of_segments_input();
    void EMSCRIPTEN_KEEPALIVE clear_array_of_points_input();

    extern InputData inputData;
    extern emscripten::val CONSTANTS;
}

#endif //TPFINALAYDAII_WASM_BINDINGS_DATA_INPUT_HPP
