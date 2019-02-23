#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <wasm_bindings/algorithms.hpp>
#include <wasm_bindings/data_input.hpp>
#include <wasm_bindings/utils.hpp>
#include <wasm_bindings/errors.hpp>

#include <algor/ClosestPairOfPoints.hpp>

using namespace emscripten;
using namespace wasm_bindings;
using namespace algor;

val EMSCRIPTEN_KEEPALIVE wasm_bindings::run_closest_points() {
    if(!verify_array_of_points_input()) {
        return generateArrayOfPointsInputNotSetError();
    }

    ClosestPairOfPoints cpp(*(inputData.asArrayOfPoints));

    if(!cpp.canRun()) {
        return generateInvalidInputError();
    }

    auto res = cpp.run();

    if(res.has_value()) {
        int * data = new int[4];

        auto response = val::object();

        data[0] = res->first.getX();
        data[1] = res->first.getY();
        data[2] = res->second.getX();
        data[3] = res->second.getY();

        response.set(val("data"), val(reinterpret_cast<long>(data)));
        response.set(val("length"), val(4));

        return std::move(response);
    }

    return val::undefined();
}

EMSCRIPTEN_BINDINGS(algorithm_closest_points) {
    function("run_closest_points", &run_closest_points);
}
