#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <wasm_bindings/algorithms.hpp>
#include <wasm_bindings/data_input.hpp>
#include <wasm_bindings/utils.hpp>
#include <wasm_bindings/errors.hpp>

#include <algor/GrahamScan.hpp>

using namespace emscripten;
using namespace wasm_bindings;
using namespace algor;

val EMSCRIPTEN_KEEPALIVE wasm_bindings::run_graham_scan() {
    if(!verify_list_of_points_input()) {
        return generateListOfPointsInputNotSetError();
    }

    GrahamScan gs(*(inputData.asListOfPoints));

    if(!gs.canRun()) {
        return generateInvalidInputError();
    }

    auto res = gs.run();

    if(res.has_value()) {
        unsigned int sz = res->size()*2;
        int * data = new int[sz];

        auto response = val::object();

        unsigned int i = 0;
        while(!res->isEmpty()) {
            auto point = res->pop();
            data[i++] = point.getX();
            data[i++] = point.getY();
        }

        response.set(val("data"), val(reinterpret_cast<long>(data)));
        response.set(val("length"), val(sz));

        return std::move(response);
    }

    return val::undefined();
}

EMSCRIPTEN_BINDINGS(algorithm_graham_scan) {
    function("run_graham_scan", &run_graham_scan);
}
