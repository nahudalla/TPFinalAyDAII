#ifndef TPFINALAYDAII_WASM_BINDINGS_UTILS_HPP
#define TPFINALAYDAII_WASM_BINDINGS_UTILS_HPP

#include <emscripten/val.h>

namespace wasm_bindings {
    bool verify_progress_callback(emscripten::val const& progress);
    bool verify_list_of_points_input();
    bool verify_array_of_points_input();
    bool verify_list_of_segments_input();
}

#endif //TPFINALAYDAII_WASM_BINDINGS_UTILS_HPP
