#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <wasm_bindings/algorithms.hpp>
#include <wasm_bindings/data_input.hpp>
#include <wasm_bindings/utils.hpp>
#include <wasm_bindings/errors.hpp>

#include <algor/JarvisMarch.hpp>

using namespace emscripten;
using namespace wasm_bindings;
using namespace algor;

val EMSCRIPTEN_KEEPALIVE wasm_bindings::run_jarvis_march() {
    if(!verify_list_of_points_input()) {
        return generateListOfPointsInputNotSetError();
    }

    JarvisMarch jm(*(inputData.asListOfPoints));

    if(!jm.canRun()) {
        return generateInvalidInputError();
    }

    auto res = jm.run();

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

EMSCRIPTEN_BINDINGS(algorithm_jarvis_march) {
    function("run_jarvis_march", &run_jarvis_march);
}
